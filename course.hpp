#ifndef COURSE_HPP
#define COURSE_HPP

#include <set>
#include <map>
#include<fstream>
#include<sstream>

#include "vector_operations.hpp"
#include "polynom.hpp"
#include "multi_affine.hpp"
#include "espp.h"

using namespace std;

vector<vector<bool>> reverse(vector<vector<bool>> matrix) {
	int i, j, k, l, n = matrix.size();
	vector<bool> tmp;
	vector<vector<bool>> reverse(n, vector<bool>(n + 1));
	bool f;
	
	for (int i = 0; i < n; ++i) {
		reverse[i][i + 1] = 1;
		reverse[i][0] = matrix[i][0];
	}
	
	for (i = 0; i < n - 1; ++i) {
		if (!matrix[i][i + 1]) {
			f = false;
			for (l = i; l < n; ++l) {
				if (matrix[l][i + 1]) {
					tmp = matrix[l];
					matrix[l] = matrix[i];
					matrix[i] = tmp;
					
					tmp = reverse[l];
					reverse[l] =reverse[i];
					reverse[i] = tmp;
					
					f = true;
				}
			}
			if (!f) 
				throw "Reversing degenerate matrix!";
		}
		for (j = i + 1; j < n; ++j) {
			if (matrix[j][i + 1]) {
				matrix[j] ^= matrix[i];
				reverse[j] ^= reverse[i];
			}
		}
	}
	
	if (!matrix[n-1][n]) 
				throw "Reversing degenerate matrix!";
	
	if (n == 1)
		return reverse;
	
	for (i = 0; i < n - 1; ++i) {
		for (j = n - i; j < n + 1; ++j) {
			if (matrix[n - 2 - i][j]) {
				reverse[n - 2 - i] ^= reverse[j - 1];
			}
		}	
	} 
	
	return reverse;
}

//computer representation of affine(linear) changes of variables(in polynomials)
class affine_change {
	vector<polynom> data;
	int n;//number of variables
public:
	affine_change(const vector<vector<bool>>&);
	vector<vector<bool>> get_matrix() const;
	//return reverse change if *this is nondegenerate, else throw const char* exeption
	affine_change reverse() const;
	void set_negations(vector<bool>);
	void set_order(const vector<int>&);
	friend ostream& ::operator<<(ostream&, const affine_change&);
	const polynom& operator[](int i) const {return data[i];}
	polynom& operator[](int i) {return data[i];}
	bool operator==(const affine_change& ac) const {return (data == ac.data);}
};

//vec[i] is change of xi
affine_change::affine_change(const vector<vector<bool>>& vec) {
	if (vec.empty())
		throw "Empty initializer in affine_change(const vector<vector<bool>>&)";
	n = vec[0].size() - 1;
	int i = 0;
	for (; i < vec.size(); ++i) {
		if (n != vec[i].size() - 1)
			throw "Number of variables are different in affine_change(const vector<vector<bool>>&)";
		data.push_back(polynom(vec[i],2));
	}
	if (i != n - 1) {
		vector<bool> v(n + 1);
		for (; i < n; ++i) {
			v[i + 1] = 1;
			data.push_back(polynom(v,2));
			v[i + 1] = 0;
		}
	}
}

vector<vector<bool>> affine_change::get_matrix() const {
	vector<vector<bool>> matrix(n, vector<bool>(n + 1));
	vector<bool> tmp;
	unsigned int p = 1;
	
	for (unsigned int i = 0; i < n; ++i) {
		tmp = data[i].get_data();
		matrix[i][0] = tmp[0];
		p = 1;
		for (unsigned int j = 0; j < n; ++j) {
			matrix[i][n - j] = tmp[p];
			p *= 2;
		}	
	} 
	
	return matrix;
}


affine_change affine_change::reverse() const {
	return ::reverse(this->get_matrix());
}

void affine_change::set_negations(vector<bool> negations) {
	if (negations.size() != n)
		throw "Wrong number of variables in affine_change::set_negations(vector<bool>)";
	vector<bool> one(n);
	for (int i = 0; i < n; ++i) {
		if (negations[i]) {
			if (data[i].find(one) == -1) {//if there is not "1" in polynomial data[i]
				data[i] += one;	
			}	
		} else {
			if (data[i].find(one) != -1) {//if there is "1" in polynomial data[i]
				data[i] += one;
			}	
		}
	}
}

void affine_change::set_order(const vector<int>& order) {
	if (order.size() != n)
		throw "Wrong number of variables in affine_change::set_order(const vector<int>&)";
	vector<polynom> tmp;
	for (int i = 0; i < n; ++i) {
		tmp.push_back(data[order[i]]);	
	}
	data = tmp;
}

ostream& operator<<(ostream& out, const affine_change& a) {
	for (int i = 0; i < a.data.size(); ++i) {
		out<<'x'<<i + 1<<" = "<<a.data[i];
	}
	return out;
}

/*vector<affine_change> make_ordered_changes(int n) {
	vector<affine_change> var;
	vector<vector<bool>> vec(n,vector<bool>(n + 1));
	return var;
}*/

bool nondegenerate(vector<vector<bool>> matrix) {
	int i, j, k, l, n = matrix.size();
	vector<bool> tmp;
	bool f;
	for (i = 0; i < n - 1; ++i) {
		if (!matrix[i][i + 1]) {
			f = false;
			for (l = i; l < n; ++l) {
				if (matrix[l][i + 1]) {
					tmp = matrix[l];
					matrix[l] = matrix[i];
					matrix[i] = tmp;
					f = true;
				}
			}
			if (!f) 
				return f;
		}
		for (j = i + 1; j < n; ++j) {
			if (matrix[j][i + 1])
				matrix[j] ^= matrix[i];
		}
	}
	return matrix[n-1][n];
}

vector<affine_change> nondegenerate_linear_changes(int n) {
	//cout<<"start\n";
	vector<affine_change> var;
	vector<vector<bool>> vec(n,vector<bool>(n + 1));//, tmp;
	unsigned long int pow_2_n = pow(2,n);//, number = 1, k;//pow_2_n;
	int k;
	/*for (int i = 1; i < pow_2_n; i *= 2) {
		number *= (pow_2_n - i);
	}*/
	for (int i = 0; i < pow(2,n*n); ++i) {
		k = i;
		for (int j = 0; j < n; ++j) {
			vec[n - 1 - j] = int_to_vec(k % pow_2_n, n + 1);
			k /= pow_2_n;
		}
		if (nondegenerate(vec))
			var.push_back(vec);
		//cout<<var.back()<<endl;	
	}
	/*for (int i = 0; i < var.size(); ++i) {
		cout<<var[i]<<endl;
	}*/
	//cout<<"All changes complite\n";
	return var;
}

/*vector<affine_change> nondegenerate_linear_changes(int n) {
	vector<affine_change> var;
	vector<vector<bool>> vec(n,vector<bool>(n + 1));
	unsigned long int pow_2_n = pow(2,n), number = 1;//pow_2_n;
	for (int i = 1; i < pow_2_n; i *= 2) {
		number *= (pow_2_n - i);
	}
	for(int i = 0; i < number; ++i) {
		for (int j = 0; j < n; ++j) {
			vec[j] = int_to_vec((i + j + (j * i / (pow_2_n - 1))) % (pow_2_n - 1) + 1,n + 1);
		}
		var.push_back(vec);	
	}
	return var;
}*/

vector<affine_change> make_all_changes(int n) {
	vector<affine_change> tmp = nondegenerate_linear_changes(n), var = tmp;
	vector<bool> negations;
	for (int i = 1; i < pow(2,n); ++i) {
		negations = int_to_vec(i,n);
		for (int j = 0; j < tmp.size(); ++j) {
			tmp[j].set_negations(negations);
		}
		var.insert(var.end(), tmp.begin(), tmp.end());
	}
	//cout<<"All changes complite\n\n";
	
	return var;
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
		p += polynom((vector<bool>(1,1) + vector<bool>(pow(2,n) - 1)), 1);
	return p;
}

class equal_functions {
	int n;
	polynom representative;
	//function associated with its position in list of all n variable functions
	map<int, int> members;
	/*vector<int> changes;
	set <pair<int,int>> pairs;*/
public:
	equal_functions(const polynom&);
	int size() const {return members.size();}
	void add_function(const polynom&, int);
	void add_function(int,int );
	void add_function(const vector<bool>&, int );
	friend ostream& ::operator<<(ostream&, const equal_functions&);
	friend ofstream& ::operator<<(ofstream&, const equal_functions&);
	template<typename T>
	friend void print(T&, const equal_functions&, const vector<affine_change>&);
	friend void test(const vector<equal_functions>& vec, const vector<affine_change>& changes);
};

equal_functions::equal_functions(const polynom& p) : n(p.get_n()), representative(p) {
	/*members.insert(vec_to_int(p)); 
	changes.push_back(-1);*/
	members.insert(pair<int,int>(vec_to_int(p), -1));
}

void equal_functions::add_function(const polynom& p, int change) {
	members.insert(pair<int,int>(vec_to_int(p),change));
	/*if (members.find(vec_to_int(p)) == members.end()) {
		members.insert(vec_to_int(p));
		changes.push_back(change);
	}*/
}

void equal_functions::add_function(int member, int change) {
	members.insert(pair<int,int>(member,change));
	/*if (members.find(member) == members.end()) {
		/*members.insert(member);
		changes.push_back(change);
		members.insert(pair<int,int>(member,change));
		/*cout<<"representative = "<<representative<<" change = "<<\
		change<<" polynom added "<<polynom(int_to_vec(member,pow(2,n)),0)<<endl;
	}*/
}

void equal_functions::add_function(const vector<bool>& v, int change) {
	members.insert(pair<int,int>(vec_to_int(v),change));
	/*if (members.find(vec_to_int(v)) == members.end()) {
		members.insert(vec_to_int(v));
		changes.push_back(change);
		//cout<<changes.back();
	}*/
}

ostream& operator<<(ostream& out, const equal_functions& ef) {
	for (auto& x : ef.members) {
		out<<polynom(int_to_vec(x.first, pow(2,ef.n)),0);
	}
	
	out<<endl;
	return out;
}

ofstream& operator<<(ofstream& out, const equal_functions& ef) {
	for (auto& x : ef.members) {
		out<<polynom(int_to_vec(x.first, pow(2,ef.n)),0);
	}
	
	out<<endl;
	return out;
}

void test(const vector<equal_functions>& vec, const vector<affine_change>& changes) {
	int i = 0;
	for (auto& y : vec) {
		for (auto& x : y.members) {
			if (i > 0) {
				if ((*(y.members.begin())).first != \
				vec_to_int(polynom(int_to_vec(x.first, pow(2,y.n)),0)\
				.change_variables(changes[x.second].reverse()))) {
					throw "Very bad!\n";
				}
			}
			++i;
		}
		
		i = 0;	
	}
} 

template<typename T>
void print(T& out, const equal_functions& ef, const vector<affine_change>& changes) {
	int i = 0;
	for (auto& x : ef.members) {
		out<<i + 1<<") "<<polynom(int_to_vec(x.first, pow(2,ef.n)),0);
		if (i > 0) {
			out<<changes[x.second];
			/*if ((*(ef.members.begin())).first != \
			vec_to_int(polynom(int_to_vec(x.first, pow(2,ef.n)),0)\
			.change_variables(changes[x.second].reverse()))) {
				throw "Very bad!\n";
			}*/
		}
		else {
			out<<"representative\n";
		}
		++i;
	}
	
	//out<<"Good\n";	
	out<<endl;
}

vector<polynom> make_all_polynoms(int n) {
	vector<polynom> polynoms;
	for (int i = 0; i < pow(2,pow(2,n)); ++i) {
		polynoms.push_back(polynom(int_to_vec(i,pow(2,n)),0));
	}
	return polynoms;
}

//makes all equivalence classes of boolean functions of n variables, created from affine
//change of variables vc
vector<equal_functions> all_eq_classes(int n, const vector<affine_change>& vc) {
	ofstream mrec("mrec");
	vector<polynom> polynoms = make_all_polynoms(n);
	
	/*for (auto& x : vc) {
		cout<<x<<endl;
	}*/
	vector<equal_functions> eq_classes;
	set<int> used_functions;
	int func_num;
	mrec<<n<<'.';
	for (int i = 0; i < polynoms.size(); ++i) {
		if (used_functions.find(vec_to_int(polynoms[i])) == used_functions.end()) {
			eq_classes.push_back(polynoms[i]);
			func_num = vec_to_int(polynoms[i]);
			mrec<<func_num<<','<<-1;
			used_functions.insert(func_num);
			for (int j = 0; j < vc.size(); ++j) {
				func_num = vec_to_int(polynoms[i].change_variables(vc[j]));
				if (used_functions.find(func_num) == used_functions.end()) {
                    eq_classes.back().add_function(func_num, j);
					mrec<<','<<func_num<<','<<j;
                }
				used_functions.insert(func_num);
				//cout<<polynoms[i]<<polynoms[i].change_variables(vc[j])<<vc[j]<<endl;
			}
			mrec<<';';
            cout<<"i = "<<i<<endl;
			//cout<<eq_classes.back();
		}
	}
	return eq_classes;
}

ofstream& operator<<(ofstream& out, const vector<equal_functions>& vec) {
	for (int i = 0; i < vec.size(); ++i) {
		out<<vec[i];
	}
	return out;
}

template <typename T>
void print(T& out, const vector<equal_functions>& eq_functions, const vector<affine_change>& changes) {
	for (int i = 0; i < eq_functions.size(); ++i) {
		print(out, eq_functions[i], changes);
	}
}

void make_stat(const vector<equal_functions>& eq) {
	int max = eq.front().size(), min = max, sum = 0, tmp;
	ofstream stat("stat");
	for (auto& x : eq) {
		tmp = x.size();
		sum += tmp;
		if (tmp > max)
			max = tmp;
		else if (tmp < min)
			min = tmp;
	}
	stat<<"Number of equivalence classes = "<<eq.size();
	stat<<"\nsize of min class = "<<min;
	stat<<"\nsize of max class = "<<max;
	stat<<"\nsize of mean class = "<<float(sum) / eq.size();
}

void save_changes(const vector<affine_change>& changes, const char* str) {
	ofstream out(str);
	vector<vector<bool>> matrix;
	for (auto& x : changes) {
		matrix = x.get_matrix();
		for (unsigned int i = 0; i < matrix.size(); ++i) {
			for (unsigned int j = 0; j < matrix[i].size(); ++j) {
				out<<matrix[i][j];	
			}
			out<<',';
		}
		//out<<';';
	}
}

vector<affine_change> load_changes(const char* str) {
	ifstream in(str);
	char c = in.get();
	vector<affine_change> changes;
	vector<vector<bool>> matrix;
	vector<bool> vec;
	unsigned int n, i, j;
	
	while (in && (c != ',')) {
		vec.push_back(c - '0');
		c = in.get();
	}
	
	matrix.push_back(vec);
	n = vec.size() - 1;
	i = 1;
	c = in.get();
	
	while ((i < n)) {
		while ((c != ',')) {
			vec[j++] = (c - '0');
			c = in.get();
		}
		
		matrix.push_back(vec);
		c = in.get();
		++i;
		j = 0;
	}
	
	changes.push_back(matrix);
	
	i = 0;
	
	while (in) {
		while ((i < n)) {
			while ((c != ',')) {
				vec[j++] = (c - '0');
				c = in.get();
			}
			
			matrix[i++] = (vec);
			c = in.get();
			//++i;
			j = 0;
		}
		
		i = 0;
		changes.push_back(matrix);
	}
	
	return changes;
}

vector<equal_functions> parse_file(const ifstream& file, const vector<affine_change>& vec) {
	vector<equal_functions> eq;
	int function, change, n;
	char c;
	stringstream ss;
	ss << file.rdbuf();
	ss>>n>>c;
	while(ss) {
		ss>>function>>c>>change>>c;
		eq.push_back(polynom(int_to_vec(function,pow(2,n)),0));
		while (c != ';') {
			ss>>function>>c>>change>>c;
			eq.back().add_function(function, change);
		}
		//print(cout, eq.back(), vec);
	}
	if (!eq.empty())
		eq.pop_back();
	
	return eq;
}

#endif