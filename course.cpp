#include "data_structures.hpp"
using namespace std;

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
	vector<vector<bool>> vec(n,vector<bool>(n + 1));
	//vector<bool> v(1,0);
	unsigned long int pow_2_n = pow(2,n), number = 1;//pow_2_n;
	for (int i = 1; i < pow_2_n; i *= 2) {
		number *= (pow_2_n - i);
	}
	//cout<<number<<endl;
	for(int i = 0; i < number; ++i) {
		//cout<<"i = "<<i<<endl;
		for (int j = 0; j < n; ++j) {
			vec[j] = int_to_vec((i + j + (j * i / (pow_2_n - 1))) % (pow_2_n - 1) + 1,n + 1);
			//cout<<vec[j];
		}
		var.push_back(vec);
		//vec = vector<vector<bool>>(n,vector<bool>(n));	
	}
	//cout<<'k';
	return var;
}

//replacing and negation of variables
vector<affine_change> make_simple_changes(int n) {
	vector<affine_change> vec;
	//vec.reserve(factorial(n) * pow(2,n));
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

class equal_functions {
	int n;
	polynom representative;
	//function associated with its position in list of all n variable functions
	vector<int> members;
public:
	equal_functions(const polynom& p) : n(p.get_n()), representative(p) {}
	void add_function(const polynom& p) {/*members.push_back();*/}
	void add_function(int i) {members.push_back(i);}
	void add_function(const vector<bool>& v) {members.push_back(vec_to_int(v));}
};

int main() 
try {
	const int n = 2;
	
	vector<bool> v1{1,0,0,1,1,0,1,1}, v2{0,0,0,0,0,0,1,1}, v3{1,0,1,0,1}, \
	v4{0,1,1,0,0}, v5{0,1,0}, v0{0,0,0,0,0};
	
	polynom p0, p1(v1,1), p2(v2,0), p3(v3,2), p4(v4,2), \
	p5(vector<vector<bool>>{{0,1,1,0},{0,0,1,0},{0,0,0,1},{0,0,0,0}}), \
	p6(vector<vector<bool>>{{1,1}, {0,1}});
	
	vector<polynom> polynoms;
	for (int i = 0; i < pow(2,pow(2,n)); ++i) {
		polynoms.push_back(polynom(int_to_vec(i,pow(2,n)),0));
		cout<<i<<" "<<vec_to_int(polynoms.back())<<endl;
	}
	//cout<<polynoms.size()<<endl;
	
	multi_affine m1(), m2(v3,1);
	
	affine_change a(vector<vector<bool>>{{1,1,1},{1,0,1}});
	vector<affine_change> vc = make_all_changes(2);
	
		
	//cout<<"hi\n";
	/*for (int i = 0; i < vc.size(); ++i) {
		cout<<p6<<vc[i]<<p6.change_variables(vc[i])<<endl;
	}*/
	//cout<<vc.size();
	//espp e(p5.get_data(), 1);
	/*(m1 += v3) += v4;
	(e += m1) += m1;
	e = rule1(p1, v5);*/
	/*p5 += p5;
	cout<<p5;*/
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