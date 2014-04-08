#include <iostream>
#include <cmath>
#include <string>
#include <vector>
using namespace std;

//display vector<bool>
ostream& operator<<(ostream& out, const vector<bool>& pol) {
	for(int i = 0; i < pol.size(); ++i) {
		out<<pol[i]<<' ';
	}
	out<<endl;
	return out;
}

//add elements of v2 to the end of v1
vector<bool>& operator+=(vector<bool>& v1, const vector<bool>& v2) {
	v1.insert(v1.end(), v2.begin(), v2.end());
	return v1;		
}

//return vector with elements of v1 += v2
vector<bool> operator+(const vector<bool>& v1, const vector<bool>& v2) {
	vector<bool> v = v1;
	return v += v2;		
}

//bitwise or of two vector<bool> and put result to v1
vector<bool>& operator|=(vector<bool>& v1, const vector<bool>& v2) {
	if (v1.size() != v2.size())
		throw "operator| vector<bool> parametrs have different size";
	for (int i = 0; i < v1.size(); ++i) {
		v1[i] = v1[i] | v2[i];
	}
	return v1;		
}

//bitwise or of two vector<bool>
vector<bool> operator|(const vector<bool>& v1, const vector<bool>& v2) {
	vector<bool> rez = v1;
	return rez |= v2;
}

//bitwise end of two vector<bool>
vector<bool> operator&(const vector<bool>& v1, const vector<bool>& v2) {
	if (v1.size() != v2.size())
		throw "operator& vector<bool> parametrs have different size";
	vector<bool> vec(v1.size());
	for (int i = 0; i < v1.size(); ++i) {
		vec[i] = v1[i] & v2[i];
	}
	return vec;		
}

//bitwise xor of two vector<bool>
vector<bool> operator^(const vector<bool>& v1, const vector<bool>& v2) {
	if (v1.size() != v2.size())
		throw "operator| vector<bool> parametrs have different size";
	vector<bool> vec(v1.size());
	for (int i = 0; i < v1.size(); ++i) {
		vec[i] = v1[i] ^ v2[i];
	}
	return vec;		
}

//calculating n!
unsigned long int factorial(int n) {
	int res = 1;
	for (int i = 2; i <= n; i++) {
		res *= i;
	}
	return res;
}

//binary representation of x with vector<bool> with num numbers
vector<bool> int_to_vec(int x, int num) {
	vector<bool> v(num);
	for(int i = 0; i < num; ++i) {
		v[num - 1 - i] = x % 2;
		x /= 2;
	}
	return v;
}

//integer representation of vector<bool> vec
int vec_to_int(const vector<bool> & vec) {
	int ans = 0, sz = vec.size();
	for(int i = 0; i < sz; ++i) {
		if (vec[sz - 1 - i]) {
			ans += pow(2, i);
		}
	}
	return ans;
}

//comparing two vector<bool> to less or equal
bool operator<=(const vector<bool>& v1, const vector<bool>& v2) {
	for (int i = 0; i < v1.size(); ++i) {
		if (v1[i] > v2[i])
			return false;
	}
	return true;
}

//comparing two vector<bool> to less or equal
bool operator>=(const vector<bool>& v1, const vector<bool>& v2) {
	for (int i = 0; i < v1.size(); ++i) {
		if (v1[i] < v2[i])
			return false;
	}
	return true;
}

//computes down shadow of alpha: all vectors <= alpha and it has length = |alpha| - 1
vector<vector<bool>> down_shadow(vector<bool> alpha) {
	vector<vector<bool>> vec;
	vector<bool> a;
	for (int i = 0; i < alpha.size(); ++i) {
		if (alpha[i]) {
			a = alpha;
			a[i].flip();
			vec.push_back(a);
		}
	}
	return vec;
}

//computes down shadow of alpha: all vectors >= alpha and it has length = |alpha| + 1
vector<vector<bool>> up_shadow(vector<bool> alpha) {
	vector<vector<bool>> vec;
	vector<bool> a;
	for (int i = 0; i < alpha.size(); ++i) {
		if (!alpha[i]) {
			a = alpha;
			a[i].flip();
			vec.push_back(a);
		}
	}
	return vec;
}

//computes ball radius of the one with the center at the point alpha
vector<vector<bool>> ball1(vector<bool> alpha) {
	vector<vector<bool>> vec;
	vector<bool> a;
	
	for (int i = 0; i < alpha.size(); ++i) {
		if (alpha[i]) {
			a = alpha;
			a[i].flip();
			vec.push_back(a);
		}
	}
	
	vec.push_back(alpha);
	
	for (int i = 0; i < alpha.size(); ++i) {
		if (!alpha[i]) {
			a = alpha;
			a[i].flip();
			vec.push_back(a);
		}
	}
	return vec;
}

//computes shading set of n demensional boolean cube 
vector<vector<bool>> make_shadow(int n) {
	if (n == 3) {
		return vector<vector<bool>>{{0,0,0}, {1,1,1}};
	}
	if (n == 4) {
		return vector<vector<bool>>{{0,0,0,0}, {1,0,0,0}, {0,1,1,1}, {1,1,1,1}};
	}
	//else ...	
}

//computes the binary logarithm of x
int log2( unsigned int x )
{
  unsigned int ans = 0, y = x;
  while( x>>=1 ) ans++;
  if((y - pow(2, ans)) != 0)
  	return -1;
  return ans ;
}

//described later
class affine_change;

//comyter representation of Zhegalkin polynomial
class polynom {
	int num;//number of variables
	vector<vector<bool>> poly;//vector of monotone elementary conjunctions(monoms)
	vector<bool> data;//vector of polynomial
	//check num to be the degree of two
	bool make_num(unsigned int i) {
		if ((num = log2(i)) == -1)
			throw "Size of vector is not the degree of two!";	
	}
public:
	void make_poly();
	bool is_zero() const;
	void clear();
	polynom() : num(0) {};
	polynom(const vector<bool>&, int);
	polynom(const vector<vector<bool>>&);
	int find(vector<bool>) const;
	bool empty() const {return data.empty();}
	polynom& operator*=(const polynom&);
	polynom operator*(const polynom&) const;
	bool test(int i) const {return data.at(i);}
	bool test(const vector<bool>& vec) const {return data.at(vec_to_int(vec));}
	const vector<bool>& get_data() const {return data;}
	int get_n() const {return num;}
	friend ostream& ::operator<<(ostream&, const polynom&);
	polynom& operator+=(const polynom&);
	polynom& operator+=(vector<bool>);
	polynom& operator+=(const vector<vector<bool>>&);
	template <typename TYPE>
	polynom operator+(TYPE) const;
	bool operator==(const polynom&) const;
	polynom change_variables(const affine_change&) const;
	polynom make_pj(vector<bool>) const;
	polynom make_pj_up(vector<bool>) const;
};

//check polynomial of the null
bool polynom::is_zero() const{
	if ((num == 0) || (data == vector<bool>(pow(2,num))))
		return true;
	return false;
}

void polynom::clear() {
	num = 0;
	data.clear();
	poly.clear();
}

//makes poly with data
void polynom::make_poly() {
	poly.clear();
	for(int i = 0; i < data.size(); ++i) {
		if(data[i]) {
			poly.push_back(int_to_vec(i, num));
		}
	}
}

//creates polynomial with vec type == 1 means that vec is vector of function, 2 means that vec is vector of polynomial and if type == 3, vec is 
//linear function v[i] = the coefficient of xi, v[0] is coefficient of 1
polynom::polynom(const vector<bool>& vec, int type) {
	if (type < 0 || type > 2)
		throw "Wrong second parameter in polynom constructor it must be from 0 to 2!";
	if (!(vec == vector<bool>(0)) && (type == 0)) {
		make_num(vec.size());
		data = vec;
		int len = vec.size(), j;
		for (int i = 0; i < log2(len); ++i) {
			j = len - 1;
			while (j > 0) {
        		for (int k = 0; k < pow(2.0,i); ++k) {
      	      	data[j - k] = (data[j - k] ^ data[j - k - pow(2.0,i)]);
	   	 	}
        		j = j - pow(2, (i + 1));
    		}
		}
		make_poly();
	}
	if (!(vec == vector<bool>(0)) && (type == 1)) {
		make_num(vec.size());
		data = vec;
		make_poly();
	}
		
	if (!(vec == vector<bool>(0)) && (type == 2)) {
		num = vec.size() - 1;
		data = vector<bool>(pow(2,num));
		vector<bool> v(num);
		if (vec[0]) {
			data[0] = 1;
			poly.push_back(v);
		}
		for (int i = 1; i < vec.size(); ++i) {
			if (vec[i]) {
				v[i - 1] = 1;
				data[vec_to_int(v)] = 1;
				poly.push_back(v);
				v = vector<bool>(num);
			}
		}
	}
}

//creates polynomial with vector of monotone elementary conjunctions
polynom::polynom(const vector<vector<bool>>& vec) {
	if (!vec.empty()) {
		num = vec[0].size();
		data = vector<bool>(pow(2,num));
		for (int i = 0; i < vec.size(); ++i) {
			if (num != vec[i].size())
				throw "Number of variables are different in polynom(vector<vector<bool>>&)";
			poly.push_back(vec[i]);
			data[vec_to_int(vec[i])].flip();
		}
	}
}

//find monom in poly
int polynom::find(vector<bool> monom) const {
		for(int i = 0; i < poly.size(); ++i) {
			if (poly[i] == monom) {
				return i;
			}
		}
		return -1;
}

//displays polynomial 
ostream& operator<<(ostream& out,const polynom& p) {
	if (p.is_zero()) {
		out<<"0\n";
		return out;
	}
	vector<bool> vec;
	bool first = true;
	for (int i = p.data.size() - 1; i >= 0; --i) {
		if (p.data[i]) {
			vec = int_to_vec(i, p.num);
			if (!first) 
				out<<" + ";
			else
				first = false;
			for(int j = 0; j < p.num; ++j) {	
				if (vec[j])
					out<<'x'<<(j + 1);
			}
			if (vec == vector<bool>(vec.size(),0))
				out<<'1';
		}
	}
	out<<"\n";
	return out;
}

//multiplies polynomials and put result to *this
polynom& polynom::operator*=(const polynom& p) {
	if (is_zero())
		return *this;
	if (p.is_zero()) {
		data.clear();
		poly.clear();
		num = 0;
		return *this;
	}
	vector<bool> vec(pow(2,num), 0);
	for (int i = 0; i < poly.size(); ++i) {
		for (int j = 0; j < p.poly.size(); ++j) {
			vec[vec_to_int(poly[i] | p.poly[j])].flip();
		}
	}
	poly.clear();
	for (int i = 0; i < vec.size(); ++i) {
		if (vec[i]) {
			poly.push_back(int_to_vec(i,num));
			data[i] = 1;
		} else {
			data[i] = 0;
		}
	}
	return *this;
}

//multiplies polynomials
polynom polynom::operator*(const polynom& p) const {
	polynom rez = *this;
	return rez *= p;
}

//adds polynomials and put result to *this
polynom& polynom::operator+=(const polynom& p) {
	if (p.is_zero()) {
		return *this;
	}	
	else if (is_zero()) {
		num = p.get_n();
		data = p.data;
		poly = p.poly;
		return *this;
	}
	else {
		if (num != p.get_n()) {
			cout<<num<<" "<<*this<<p.num<<" "<<p;
			throw "Number of variables are different in polynom& operator+=(const polynom&)";			
		}
		for (int i = 0; i < p.data.size(); ++i) {
			data[i] = data[i] ^ p.data[i];
		}
		make_poly();
		return *this;
	}
}

// adds monom to *this
polynom& polynom::operator+=(vector<bool> vec) {
	if (is_zero()) {
		num = vec.size();
		data = vector<bool>(pow(2,num));
	}
	else if (num != vec.size())
		throw "Number of variables are different in polynom& operator+=(vector<bool>)";
	data[vec_to_int(vec)].flip();
	make_poly();
	return *this;
}

//adds vector of monoms to *this
polynom& polynom::operator+=(const vector<vector<bool>>& vec) {
	for (int i = 0; i < vec.size(); ++i) {
		*this += vec[i];
	}
	return *this;
}

template <typename TYPE>
polynom polynom::operator+(TYPE t) const {
	polynom rez = *this;
	return rez += t;
}

bool polynom::operator==(const polynom& p) const  {
	if (is_zero() && p.is_zero())
		return true;
	if (data != p.data) 
		return false;
	return true;
}

//computer representation of affine(linear) changes of variables(in polynomials)
class affine_change {
	vector<polynom> data;
	int num;//number of variables
public:
	affine_change(const vector<vector<bool>>&);
	affine_change& add_change(vector<bool>);
	//affine_change operator+(vector<bool>);
	friend ostream& ::operator<<(ostream&, const affine_change&);
	const polynom& operator[](int i) const {return data[i];}
	polynom& operator[](int i) {return data[i];}
};

//vec[i] is change of xi
affine_change::affine_change(const vector<vector<bool>>& vec) {
	if (vec.empty())
		throw "Empty initializer in affine_change(const vector<vector<bool>>&)";
	num = vec[0].size() - 1;
	int i = 0;
	for (; i < vec.size(); ++i) {
		if (num != vec[i].size() - 1)
			throw "Number of variables are different in affine_change(const vector<vector<bool>>&)";
		data.push_back(polynom(vec[i],2));
	}
	if (i != num - 1) {
		vector<bool> v(num + 1);
		for (; i < num; ++i) {
			v[i + 1] = 1;
			data.push_back(polynom(v,2));
			v[i + 1] = 0;
		}
	}
}

/*affine_change& affine_change::add_change(vector<bool> vec) {
	
}*/

ostream& operator<<(ostream& out, const affine_change& a) {
	for (int i = 0; i < a.data.size(); ++i) {
		out<<'x'<<i + 1<<" = "<<a.data[i];
	}
	return out;
}

vector<affine_change> make_all_changes(int n) {
	vector<affine_change> var;
	vector<vector<bool>> vec(n,vector<bool>(n));
	unsigned long int pow_2_n = pow(2,n), number = 1;//pow_2_n;
	for (int i = 1; i < pow_2_n; i *= 2) {
		number *= (pow_2_n - i);
	}
	//cout<<number<<endl;
	for(int i = 0; i < number; ++i) {
		for (int j = 0; j < n; ++j) {
			/*for (int k = 0; k < n; ++k) {
				vec[j][k] = int_to_vec(i,n);
			}*/
			vec[j] = int_to_vec((j + 1) % (pow_2_n - j - 1),n);
			cout<<vec[j];
		}
		var.push_back(vec);
		vec = vector<vector<bool>>(n,vector<bool>(n));	
	}
	return var;
}

//replacing and negation of variables
vector<affine_change> make_simple_changes(int n) {
	vector<affine_change> vec;
	vec.reserve(factorial(n) * pow(2,n));
	unsigned long int pow_2_n = pow(2,n), number = pow_2_n;
	for (int i = 1; i < pow_2_n; i *= 2) {
		number *= (pow_2_n - i);
	}
	//cout<<number<<endl;
	for(int i = 0; i < number; ++i) {
		for (int j = 0; j < n; ++j) {
			
		}		
	}
	return vec;
}

polynom polynom::change_variables(const affine_change& var) const {
	polynom p, tmp;
	for (int i = 0; i < poly.size(); ++i) {
		for (int j = 0; j < poly[i].size(); ++j) {
			if (poly[i][j]) {
				if (tmp.is_zero()) {
					tmp += var[j];
				} else {
					tmp *= var[j];
				}
			}
		}
		p += tmp;
		tmp.clear();
	}
	if (data[0])
		p += polynom((vector<bool>(1,1) + vector<bool>(pow(2,num) - 1)), 1);
	return p;
}

polynom polynom::make_pj(vector<bool> alpha) const {
	if (data.empty())
		return polynom();
	vector<vector<bool>> ball = ball1(alpha);
	polynom p;
	for (int i = 0; i < ball.size(); ++i) {
		if (data[vec_to_int(ball[i])])
			p += ball[i];
	}
	p.make_poly();
	return p;
}

polynom polynom::make_pj_up(vector<bool> alpha) const {
	if (data.empty())
		return polynom();
	vector<vector<bool>> up = up_shadow(alpha);
	polynom p;
	for (int i = 0; i < up.size(); ++i) {
		if (data[vec_to_int(up[i])])
			p += up[i];
	}
	p.make_poly();
	return p;
}

//computer representation of multi affine function(product of affine(linear) functions)
class multi_affine {
	int num;
	vector<vector<bool>> data;
public:
	//bool is_one() {return (num == 0 || data.empty());}
	multi_affine(const vector<vector<bool>>&);
	multi_affine() {}
	multi_affine(const vector<bool>&, unsigned int);
	bool empty() const {return data.empty();}
	bool operator==(const multi_affine& m) const {return data == m.data;}
	friend ostream& ::operator<<(ostream&, const multi_affine&);
	int find(vector<bool> func) const;
	multi_affine& operator*=(vector<bool>);
	polynom to_polynom() const;
};

multi_affine::multi_affine(const vector<vector<bool>> &vec) {
	if (vec.empty())
		throw "Empty initializer in multiaffine(const vector<vector<bool>>&)";
	num = vec[0].size() - 1;
	for (int i = 0; i < vec.size(); ++i) {
		if (num != vec[i].size()) {
			throw "Number of variables are different in \
			multiaffine(const vector<vector<bool>>&)";
		}
		if (vec[i] == vector<bool>(num + 1)) {
			data.clear();
			break;
		}
		if ((find(vec[i]) != -1) || \
		(vec[i] == (vector<bool>(1,1) + vector<bool>(num))) && \
		(!data.empty() || (i != vec.size() - 2))) {
			continue;
		}
		data.push_back(vec[i]);
	}
}

int multi_affine::find(vector<bool> func) const {
	for(int i = 0; i < data.size(); ++i) {
		if (data[i] == func) {
			return i;
		}
	}
	return -1;
}
	
multi_affine& multi_affine::operator*=(vector<bool> func) {
	//multiplication to 1 do nothing
	if (!(func.empty() || data.empty() || func == (vector<bool>(1,1) + vector<bool>(num)))) {
		if (func == vector<bool>(func.size())) {
			data.clear();
		}
		else if (find(func) == -1) {
			data.push_back(func);
		}
	}

	return *this;
}

//if type == 1 construct affine function else construct product of xi if vec[i] == 1
multi_affine::multi_affine(const vector<bool>& vec,unsigned int type) {
	if (type > 1)
		throw "Bad type value in multi_affine(const vector<bool>& vec,unsigned int type)";
	if ((vec != vector<bool>(vec.size()) && (vec.size() != 0))) {
		if (type == 0) {
			num = vec.size();
			vector<bool> v(num + 1);
			for (int i = 0; i < num; ++i) {
				if (vec[i]) {
					v[i + 1] = 1;
					data.push_back(v);
					v = vector<bool>(num + 1);
				}
			}
		} else {
			num = vec.size() - 1;
			data.push_back(vec);
		}
	}
}

ostream& operator<<(ostream& out, const multi_affine& m) {
	if (m.data.empty()) {
		out<<'0';
		return out;
	}
	bool first = true;
	for (int i = 0; i < m.data.size(); ++i) {
		first = true;
		if (m.data.size() != 1)
			out<<'(';
		for (int j = 1; j <= m.num; ++j) {
			if (m.data[i][j]) {
				if (!first)
					out<<" + ";
				else {
					first = false;
					//out<<'(';
				}
				out<<'x'<<j;
			}
		}
		if (m.data[i][0]) {
			if (!first)
				out<<" + ";
			if ((i != 0) && first) {
				continue;
			}
			/*if (first)
				out<<'(';*/
			out<<1;
		}
		if (m.data.size() != 1)
			out<<')';	
	}
	//out<<endl;
	return out;
}

polynom multi_affine::to_polynom() const {
	if (data.empty())
		return polynom();
	polynom p(data[0], 2);
	for (int i = 1; i < data.size(); ++i) {
		p *= polynom(data[i], 2);
	}
	return p;
}

//comuter representation of "exor sums of pseudo priducts" (exor sums of multiaffine functions)
class espp {
	vector<multi_affine> data;
public:
	espp() {};
	espp (vector<bool>, bool); 
	bool empty() {return data.empty();}
	friend ostream& ::operator<<(ostream&, const espp&);
	int find(const multi_affine& m) {
		for(int i = 0; i < data.size(); ++i) {
			if (data[i] == m) {
				return i;
			}
		}
		return -1;
	}
	espp& operator+=(const multi_affine& m) {
		int i;
		if ((i = find(m)) == -1)
			data.push_back(m);
		else
			data.erase(data.begin() + i);
		return *this;
	}
	espp& operator+=(const espp&);
	template <typename TYPE>
	espp operator+(const TYPE m) const{
		espp e = *this;
		return e += m;
	}
}; 

ostream& operator<<(ostream& out, const espp& e) {
	if (e.data.empty()) {
		return out;
	}
	bool first = true;
	for (int i = 0; i < e.data.size(); ++i) {
		if (!first)
			out<<" + ";
		else first = false;
		out<<e.data[i];
	}
	out<<endl;
	return out;
}

//add espp e to *this
espp& espp::operator+=(const espp& e) {
	for (int i = 0; i < e.data.size(); ++i) {
		if (find(e.data[i]) == -1)
			data.push_back(e.data[i]);
		else
			data.erase(data.begin() + i);	
	}
	return *this;
}

//rule of conversion polynom to espp	
espp rule1(const polynom& p, const vector<bool>& alpha, bool with_alpha = true) {
	if (p.get_n() != alpha.size())
		throw "Number of variables are different in rule1";
	espp e;
	vector<vector<bool>> up = up_shadow(alpha);
	if (with_alpha) up.push_back(alpha);
	vector<bool> vec(alpha.size()), v(alpha.size() + 1);
	for (int i = 0; i < up.size(); ++i) {
		if (p.test(up[i])) {
			if (alpha == up[i])
				v[0] = 1;
			else
				vec |= (alpha ^ up[i]);
		}
	}
	for (int i = 0; i < vec.size(); ++i) {
		v[i + 1] = vec[i];
	}
	if (!with_alpha) v[0].flip();
	e += multi_affine(alpha,0) *= v;
	return e;	
}

//main algorithm 
espp::espp(vector<bool> vec, bool type) {
	polynom p(vec, type), pj, pj_up, pj_down, p_rest;
	espp q, y_up, y_down;
	vector<vector<bool>> t = make_shadow(p.get_n());
	for (int j = 0; j < t.size(); ++j) {
		pj = p.make_pj(t[j]);
		pj_up = p.make_pj_up(t[j]);	
		if (!p.test(t[j]))
			pj_up += t[j];
		pj_down = pj + pj_up;
		y_up = rule1(pj_up, t[j]);
		cout<<"p = "<<p<<"j = "<<j<<" t[j] = "<<t[j]<<"pj = "<<pj<<"pj_up = "<<pj_up\
		<<"y_up = "<<y_up<<"\n\n";
		
		
		p += (pj + p_rest);
		q += (y_up + y_down);
	}
}

int main() 
try {
	vector<bool> v1{1,0,0,1,1,0,1,1}, v2{0,0,0,0,0,0,1,1}, v3{1,0,1,0,1}, \
	v4{0,1,1,0,0}, v5{0,1,0}, v0{0,0,0,0,0};
	polynom p0, p1(v1,1), p2(v2,0), p3(v3,2), p4(v4,2), \
	p5(vector<vector<bool>>{{0,1,1,0},{0,0,1,0},{0,0,0,1},{0,0,0,0}}), \
	p6(vector<vector<bool>>{{1,1},{0,0},{1,0},{0,1}});
	multi_affine m1, m2(v3,1);
	affine_change a(vector<vector<bool>>{{1,1,1},{1,0,1}});
	vector<affine_change> vc = make_all_changes(2);
	/*for (int i = 0; i < vc.size(); ++i) {
		cout<<vc[i]<<endl;
	}
	cout<<vc.size();*/
	/*espp e(p5.get_data(), 1);
	(m1 *= v3) *= v4;
	(e += m1) += m1;*/
	//e = rule1(p1, v5);
	/*p5 += p5;
	cout<<p5;*/
	//cout<<e;
	//m2 *= vector<bool>{1,0,0,0,0};
	//m2 *= vector<bool>{0,1,0,0,0};
	//cout<<m2<<endl;
	//cout<<p0<<(p0 += polynom(vector<bool>{0,1,0},2));
	//cout<<p6<<a<<p6.change_variables(a);
	//cout<<p1.get_data()<<p1<<endl<<p2.get_data()<<p2<<endl<<(p1 * p2)<<endl;
	//cout<<p3<<p4<<(p3 *= p4)<<p1<<p2<<p1*p2;
} 
catch(const char* str) {
	cout<<str<<endl;
}