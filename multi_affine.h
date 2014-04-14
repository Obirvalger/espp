#ifndef MULTI_AFFINE_H
#define MULTI_AFFINE_H

#include <iostream>
#include <cmath>
#include <vector>
#include "vector_operations.hpp"
#include "polynom.hpp"

using namespace std;

//computer representation of multi affine function(product of affine(linear) functions)
class multi_affine {
	int n;//number of variables
	vector<vector<bool>> data;/*consists of vectors there is "1" at i position in vector if 
	the corresponding coefficient boolean variable == 1, vector[0] is a coefficient of "1"*/
public:
	multi_affine(const vector<vector<bool>>&);
	//one as a multiaffine function of n variables
	multi_affine(unsigned int num) : n(num) {data.push_back(vector<bool>(1,1) + vector<bool>(n));}
	multi_affine(const vector<bool>&, unsigned int);
	bool empty() const {return data.empty();}
	bool operator==(const multi_affine& m) const {return data == m.data;}
	friend ostream& ::operator<<(ostream&, const multi_affine&);
	int find(vector<bool> func) const;
	multi_affine& operator*=(vector<bool>);
	operator polynom() const;
};

#endif