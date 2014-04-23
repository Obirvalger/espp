#ifndef EQUIVALENCE_CLASSES_H
#define EQUIVALENCE_CLASSES_H

#include <set>
#include <map>
#include<fstream>
#include<sstream>

#include "affine_changes.hpp"
#include "vector_operations.hpp"
#include "polynom.hpp"
#include "multi_affine.hpp"
#include "espp.h"

using namespace std;

class equal_functions {
	int n;
	//function associated with its position in list of all n variable functions
	//second int is number of change, generated this function from representative
	map<int, int> members;
public:
	equal_functions(const polynom&);
	polynom get_representative() const;
	int size() const {return members.size();}
	void add_function(const polynom&, int);
	void add_function(int,int );
	void add_function(const vector<bool>&, int );
	friend ostream& ::operator<<(ostream&, const equal_functions&);
	friend ofstream& ::operator<<(ofstream&, const equal_functions&);
	template<typename T>
	friend void print(T&, const equal_functions&, const vector<affine_change>&);
	friend void test(const vector<equal_functions>& vec, const vector<affine_change>& changes);
	pair<polynom, int> shortest() const;
};

#endif