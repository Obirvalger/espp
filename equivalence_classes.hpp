#ifndef EQUIVALENCE_CLASSES_HPP
#define EQUIVALENCE_CLASSES_HPP

#include "equivalence_classes.h"

using namespace std;

equal_functions::equal_functions(const polynom& p) : n(p.get_n()) {
	members.insert(pair<int,int>(vec_to_int(p), -1));
}

polynom equal_functions::get_representative() const {
	return polynom(int_to_vec((*(members.begin())).first,pow(2, n)),0);
}

void equal_functions::add_function(const polynom& p, int change) {
	members.insert(pair<int,int>(vec_to_int(p),change));
}

void equal_functions::add_function(int member, int change) {
	members.insert(pair<int,int>(member,change));
}

void equal_functions::add_function(const vector<bool>& v, int change) {
	members.insert(pair<int,int>(vec_to_int(v),change));
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

void make_summary(const vector<equal_functions>& vec, const char* str) {
	ofstream out(str);	
	out<<"Number of equivalence classes = "<<vec.size()<<"\n";
	for (unsigned int i = 0; i < vec.size(); ++i) {
		out<<"\n"<<i + 1<<" class: representative is\n  "<<vec[i].get_representative();
		out<<"  there are "<<vec[i].size()<<" functions in this class\n";
	}
}

void test(const vector<equal_functions>& vec, const vector<affine_change>& changes) {
	int i = 0;
	set<int> representatives;
	for (auto& y : vec) {
		representatives.insert((*(y.members.begin())).first);
		for (auto& x : y.members) {
			if (i > 0) {
				if ((*(y.members.begin())).first != \
				vec_to_int(polynom(int_to_vec(x.first, pow(2,y.n)),0)\
				.change_variables(changes[x.second].reverse()))) {
					throw "Wrong equivalence classes!";
				}
			}
			++i;
		}
		
		i = 0;	
	}
	
	if (representatives.size() != vec.size()) 
		throw "Wrong equivalence classes!";
} 

template<typename T>
void print(T& out, const equal_functions& ef, const vector<affine_change>& changes) {
	int i = 0;
	for (auto& x : ef.members) {
		out<<i + 1<<") "<<polynom(int_to_vec(x.first, pow(2,ef.n)),0);
		if (i > 0) {
			out<<changes[x.second];
		}
		else {
			out<<"representative\n";
		}
		++i;
	}
		
	out<<endl;
}

//makes all equivalence classes of boolean functions of n variables, created from affine
//change of variables vc
vector<equal_functions> all_eq_classes(int n, const vector<affine_change>& vc) {
	ofstream mrec("mrec");
	vector<polynom> polynoms = make_all_polynoms(n);
	
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
			}
			mrec<<';';
            cout<<"i = "<<i<<endl;
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
	}
	if (!eq.empty())
		eq.pop_back();
	
	return eq;
}

#endif