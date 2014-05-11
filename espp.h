#ifndef ESPP_H
#define ESPP_H

#include <iostream>
#include <cmath>
#include <vector>

#include "vector_operations.hpp"
#include "polynom.hpp"
#include "multi_affine.hpp"

using namespace std;

//comuter representation of "exor sums of pseudo products" (exor sums of multiaffine functions)
class espp {
	int n;
	vector<multi_affine> data;
public:
	espp() : n(0), data() {};
	espp (vector<bool>, bool); 
	bool empty() const {return (n == 0 || data.empty());}
	template <typename T>
	friend T& ::operator<<(T&, const espp&);
	int find(const multi_affine&);
	espp& operator+=(const multi_affine& m);
	espp& operator+=(const espp&);
	template <typename TYPE>
	espp operator+(const TYPE m) const;
};

#endif
