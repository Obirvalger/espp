#ifndef EQUIVALENCE_CLASSES_HPP
#define EQUIVALENCE_CLASSES_HPP

#include "equivalence_classes.h"
#include <algorithm>

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

class eq_cmp {
public:
	bool operator()(const equal_functions& eq1, const equal_functions& eq2) const {
		return (eq1.shortest().first.length() < eq2.shortest().first.length());
	}
};

espp make_espp(const polynom& form, const affine_change& var) {
	espp e;
	multi_affine tmp(form.get_n());
	vector<vector<bool>> poly = form.get_poly(), matrix = var.get_matrix();
	
	//cout<<tmp<<endl;
	
	for (int i = 0; i < poly.size(); ++i) {
		for (int j = 0; j < poly[i].size(); ++j) {
			if (poly[i][j]) {
				tmp *= matrix[j];
			}
		}
		
		e += tmp;
		
		//cout<<"tmp = "<<tmp<<endl;
		
		tmp.set_one();
	}
	
	//cout<<"e = "<<e<<"\n\n";
		
	return e;	
}

void make_summary(vector<equal_functions> vec, const char* str) {
	sort(vec.begin(), vec.end(), eq_cmp());
	ofstream out(str);	
	out<<"Number of equivalence classes = "<<vec.size()<<"\n";
	for (unsigned int i = 0; i < vec.size(); ++i) {
		out<<"\n"<<i + 1<<" class\n  representative:\n  "<<vec[i].get_representative();
		out<<"  there are "<<vec[i].size()<<" functions in this class\n";
		out<<"  shortest function:\n  "<<vec[i].shortest().first;
		out<<"  length = "<<vec[i].shortest().first.length()<<endl;
	}
}

void make_tex_table(vector<equal_functions> vec, const vector<affine_change> changes, const char* str) {
	ofstream out(str);
	sort(vec.begin(), vec.end(), eq_cmp());
	for (unsigned int i = 0; i < vec.size(); ++i) {
		out<<"\n"<<i + 1<<" & $ "<<vec[i].get_representative();
		out<<"$ & "<<vec[i].size()<<" & "<<vec[i].shortest().first.length();
		out<<" &\n";
		//out<<vec[i].shortest().first;
		if (vec[i].shortest().second != -1) {
			//out<<changes[vec[i].shortest().second].reverse();
			out<<"$ "<<make_espp(vec[i].shortest().first, changes[vec[i].shortest().second].reverse());
			out<<"$\n";
		}
		else 
			out<<"$ "<<vec[i].get_representative()<<"$\n";
		out<<"& "<<(double(vec[i].shortest().first.length()) / pow(2,vec[i].get_n()));
		out<<" \\\\ \\hline"<<endl;
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
			eq_classes.push_back(equal_functions(polynoms[i]));
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
		eq.push_back(equal_functions(polynom(int_to_vec(function,pow(2,n)),0)));
		while (c != ';') {
			ss>>function>>c>>change>>c;
			eq.back().add_function(function, change);
		}
	}
	if (!eq.empty())
		eq.pop_back();
	
	return eq;
}

pair<polynom, int> equal_functions::shortest() const {
	//polynom p = polynom(int_to_vec((*(members.begin())).first, pow(2,n)),0);
	unsigned int minl = polynom(int_to_vec((*(members.begin())).first, pow(2,n)),0).length();
	unsigned int val = minl;
	pair<int, int> function = (*(members.begin()));
	
	for (auto& x : members) {
		val = polynom(int_to_vec(x.first, pow(2,n)),0).length();
		if (minl > val) {
			minl = val;
			function = x;
		}
	}
	
	return pair<polynom, int>(polynom(int_to_vec(function.first, pow(2,n)),0), function.second);
}

vector<pair<polynom, int>> shortest(const vector<equal_functions>& eq, const vector<affine_change>& ac) {
	vector<pair<polynom, int>> vec;
	ofstream out("short_functions");
	
	for (auto& x : eq) {
		vec.push_back(x.shortest());
		/*cout<<x.get_representative();
		out<<x.get_representative();
		out<<vec.back().first;
		if (vec.back().second != -1)
			out<<vec.back().first.change_variables(ac[vec.back().second].reverse());
		else
			out<<vec.back().first;
		out<<"\n";*/
	}
	
	return vec;
}

#endif