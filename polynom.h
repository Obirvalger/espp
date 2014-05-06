#ifndef POLYNOM_H
#define POLYNOM_H

#include <iostream>
#include <cmath>
#include <vector>

#include "vector_operations.h"

using namespace std;

class affine_change;

//comyter representation of Zhegalkin polynomial
class polynom {
	int n;//number of variables
	vector<vector<bool>> poly;//vector of monotone elementary conjunctions(monoms)
	vector<bool> data;//vector of polynomial
	//check n to be the degree of two
	bool make_n(unsigned int i) {
		if ((n = _log2(i)) == -1)
			throw "Size of vector is not the degree of two!";	
	}
public:
	void make_poly();
	bool is_zero() const;
	unsigned int length();
	void clear();
	//empy polynom means zero, zero can be interpreted as polynomial with any number of variables
	polynom() : n(0) {};
	polynom(const vector<bool>&, int);
	operator vector<bool>() const;
	polynom(const vector<vector<bool>>&);
	int find(vector<bool>) const;
	bool empty() const {return data.empty();}
	polynom& operator*=(const polynom&);
	polynom operator*(const polynom&) const;
	bool test(int i) const {return data.at(i);}
	bool test(const vector<bool>& vec) const {return data.at(vec_to_int(vec));}
	const vector<bool>& get_data() const {return data;}
	const vector<vector<bool>>& get_poly() const {return poly;}
	bool has_neg() const {return data[0];}
	int get_n() const {return n;}
	friend ostream& ::operator<<(ostream&, const polynom&);
	polynom& operator+=(const polynom&);
	polynom& operator+=(vector<bool>);
	polynom& operator+=(const vector<vector<bool>>&);
	template <typename TYPE>
	polynom operator+(TYPE) const;
    //polynom operator+(const polynom&) const;
	bool operator==(const polynom&) const;
	polynom change_variables(const affine_change&) const;
	polynom make_pj(vector<bool>) const;
	polynom make_pj_up(vector<bool>) const;
};

#endif
