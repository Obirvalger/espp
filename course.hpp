#ifndef COURSE_HPP
#define COURSE_HPP

#include <set>
#include<fstream>

#include "vector_operations.hpp"
#include "polynom.hpp"
#include "multi_affine.hpp"
#include "espp.h"

using namespace std;

//computer representation of affine(linear) changes of variables(in polynomials)
class affine_change {
	vector<polynom> data;
	int num;//number of variables
public:
	affine_change(const vector<vector<bool>>&);
	//affine_change& add_change(vector<bool>);
	//affine_change operator+(vector<bool>);
	void set_negations(vector<bool>);
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

void affine_change::set_negations(vector<bool> negations) {
	if (negations.size() != num)
		throw "Wrong number of variables in affine_change::set_negations(vector<bool>)";
	vector<bool> one(num);
	for (int i = 0; i < num; ++i) {
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

ostream& operator<<(ostream& out, const affine_change& a) {
	for (int i = 0; i < a.data.size(); ++i) {
		out<<'x'<<i + 1<<" = "<<a.data[i];
	}
	return out;
}

vector<affine_change> nondegenerate_linear_changes(int n) {
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
}

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
		
	return var;
}

/* //replacing and negation of variables
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
}*/

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
	set<int> members;
public:
	equal_functions(const polynom& p) : n(p.get_n()), representative(p) {members.insert(vec_to_int(p));}
	void add_function(const polynom& p) {members.insert(vec_to_int(p));}
	void add_function(int i) {members.insert(i);}
	void add_function(const vector<bool>& v) {members.insert(vec_to_int(v));}
	friend ostream& ::operator<<(ostream&, const equal_functions&);
	friend ofstream& ::operator<<(ofstream&, const equal_functions&);
};

ostream& operator<<(ostream& out, const equal_functions& ef) {
	for (int x : ef.members) {
		out<<polynom(int_to_vec(x, pow(2,ef.n)),0);
	}
	
	out<<endl;
	return out;
}

ofstream& operator<<(ofstream& out, const equal_functions& ef) {
	for (int x : ef.members) {
		out<<polynom(int_to_vec(x, pow(2,ef.n)),0);
	}
	
	out<<endl;
	return out;
}

vector<polynom> make_all_polynoms(int n) {
	vector<polynom> polynoms;
	for (int i = 0; i < pow(2,pow(2,n)); ++i) {
		polynoms.push_back(polynom(int_to_vec(i,pow(2,n)),0));
	}
	return polynoms;
}

//makes all equivalence class of boolean functions of n variables, created from affine
//change of variables vc
vector<equal_functions> all_eq_classes(int n, const vector<affine_change>& vc) {
	vector<polynom> polynoms = make_all_polynoms(n);
	
	/*for (auto& x : vc) {
		cout<<x<<endl;
	}*/
	vector<equal_functions> eq_classes;
	set<int> used_functions;
	int func_num;
	for (int i = 0; i < polynoms.size(); ++i) {
		if (used_functions.find(vec_to_int(polynoms[i])) == used_functions.end()) {
			eq_classes.push_back(polynoms[i]);
			for (int j = 0; j < vc.size(); ++j) {
				func_num = vec_to_int(polynoms[i].change_variables(vc[j]));
				eq_classes.back().add_function(func_num);
				used_functions.insert(func_num);
			}
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

#endif