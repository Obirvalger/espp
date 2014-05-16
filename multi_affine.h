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
	explicit multi_affine(unsigned int num) : n(num) {data.push_back(vector<bool>(1,1) + vector<bool>(n));}
	multi_affine(const vector<bool>&, unsigned int);
	//make *this an one(1)
	void set_one() {data.clear(); data.push_back(vector<bool>(1,1) + vector<bool>(n));}
	bool is_one() const {return ((data.size() == 1) && (data[0] == (vector<bool>(1,1) + vector<bool>(n))));}
	bool empty() const {return data.empty();}
	int get_n() const {return n;}
    vector<vector<bool>> get_data() const {return data;}
	bool operator==(const multi_affine& m) const {return data == m.data;}
	template<typename T>
	friend T& ::operator<<(T&, const multi_affine&);
	int find(vector<bool> func) const;
	multi_affine& operator*=(vector<bool>);
	operator polynom() const;
    bool is_affine() const {return (data.size() > 1) ? false : true;}
};

#endif
