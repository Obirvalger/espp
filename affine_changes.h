#ifndef AFFINE_CHANGES_H
#define AFFINE_CHANGES_H

#include <set>
#include <map>
#include<fstream>
#include<sstream>

#include "vector_operations.hpp"
#include "polynom.hpp"
#include "multi_affine.hpp"
#include "espp.h"

using namespace std;

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

#endif