#ifndef POLYNOM_HPP
#define POLYNOM_HPP

#include <iostream>
#include <cmath>
#include <vector>

#include "polynom.h"
#include "vector_operations.hpp"

using namespace std;

//check polynomial to be zero
bool polynom::is_zero() const{
	if ((n == 0) || (data == vector<bool>(pow(2,n))) || data.empty())
		return true;
	return false;
}

void polynom::clear() {
	n = 0;
	data.clear();
	poly.clear();
}

//makes poly with data
void polynom::make_poly() {
	poly.clear();
	for(int i = 0; i < data.size(); ++i) {
		if(data[i]) {
			poly.push_back(int_to_vec(i, n));
		}
	}
}

//creates polynomial with vec type == 1 means that vec is vector of function, 2 means 
//that vec is vector of polynomial and if type == 3, vec is 
//linear function v[i] = the coefficient of xi, v[0] is coefficient of 1
polynom::polynom(const vector<bool>& vec, int type) {
	if (type < 0 || type > 2)
		throw "Wrong second parameter in polynom constructor it must be from 0 to 2!";
	if (!(vec == vector<bool>(0)) && (type == 0)) {
		make_n(vec.size());
		data = vec;
		int len = vec.size(), j;
		for (int i = 0; i < log2(len); ++i) {
			j = len - 1;
			while (j > 0) {
        		for (int k = 0; k < pow(2.0,i); ++k) {
      	      	data[j - k] = (data[j - k] ^ data[j - k - pow(2.0,i)]);
	   	 	}
        		j = j - pow(2, (i + 1));
    		}
		}
		make_poly();
	}
	if (!(vec == vector<bool>(0)) && (type == 1)) {
		make_n(vec.size());
		data = vec;
		make_poly();
	}
		
	if (!(vec == vector<bool>(0)) && (type == 2)) {
		n = vec.size() - 1;
		data = vector<bool>(pow(2,n));
		vector<bool> v(n);
		if (vec[0]) {
			data[0] = 1;
			poly.push_back(v);
		}
		for (int i = 1; i < vec.size(); ++i) {
			if (vec[i]) {
				v[i - 1] = 1;
				data[vec_to_int(v)] = 1;
				poly.push_back(v);
				v = vector<bool>(n);
			}
		}
	}
}

//return vector of function represented of polynomial
polynom::operator vector<bool>() const{
	vector<bool> vec = data;
	int len = vec.size(), j;
	for (int i = log2(len) - 1; i >= 0; --i) {
		j = len - 1;
		while (j > 0) {
			for (int k = 0; k < pow(2,i); ++k) {
				vec[j - k] = (vec[j - k] ^ vec[j - k - pow(2.0,i)]);
	   	 }
        	j = j - pow(2, (i + 1));
    	}
	}
	return vec;
}

//creates polynomial with vector of monotone elementary conjunctions
polynom::polynom(const vector<vector<bool>>& vec) {
	if (!vec.empty()) {
		n = vec[0].size();
		data = vector<bool>(pow(2,n));
		for (int i = 0; i < vec.size(); ++i) {
			if (n != vec[i].size())
				throw "Number of variables are different in polynom(vector<vector<bool>>&)";
			poly.push_back(vec[i]);
			data[vec_to_int(vec[i])].flip();
		}
	}
}

//find monom in poly
int polynom::find(vector<bool> monom) const {
		int i = vec_to_int(monom);
		if (data[i])
			return i;
		else 
			return -1;
}

//displays polynomial 
ostream& operator<<(ostream& out,const polynom& p) {
	if (p.is_zero()) {
		out<<"0\n";
		return out;
	}
	vector<bool> vec;
	bool first = true;
	for (int i = p.data.size() - 1; i >= 0; --i) {
		if (p.data[i]) {
			vec = int_to_vec(i, p.n);
			if (!first) 
				out<<" + ";
			else
				first = false;
			for(int j = 0; j < p.n; ++j) {	
				if (vec[j])
					out<<'x'<<(j + 1);
			}
			if (vec == vector<bool>(vec.size(),0))
				out<<'1';
		}
	}
	out<<"\n";
	return out;
}

//multiplies polynomials and put result to *this, number of varibles must be equal
polynom& polynom::operator*=(const polynom& p) {
	if (is_zero())
		return *this;
	if (p.is_zero()) {
		data.clear();
		poly.clear();
		n = 0;
		return *this;
	}
	vector<bool> vec(pow(2,n), 0);
	for (int i = 0; i < poly.size(); ++i) {
		for (int j = 0; j < p.poly.size(); ++j) {
			vec[vec_to_int(poly[i] | p.poly[j])].flip();
		}
	}
	poly.clear();
	for (int i = 0; i < vec.size(); ++i) {
		if (vec[i]) {
			poly.push_back(int_to_vec(i,n));
			data[i] = 1;
		} else {
			data[i] = 0;
		}
	}
	return *this;
}

//multiplies polynomials, number of varibles must be equal
polynom polynom::operator*(const polynom& p) const {
	polynom rez = *this;
	return rez *= p;
}

//adds polynomials and put result to *this, number of varibles must be equal
polynom& polynom::operator+=(const polynom& p) {
	if (p.is_zero()) {
		return *this;
	}	
	else if (is_zero()) {
		n = p.get_n();
		data = p.data;
		poly = p.poly;
		return *this;
	}
	else {
		if (n != p.get_n()) {
			throw "Number of variables are different in polynom& operator+=(const polynom&)";			
		}
		for (int i = 0; i < p.data.size(); ++i) {
			data[i] = data[i] ^ p.data[i];
		}
		make_poly();
		return *this;
	}
}

// adds monom to *this
polynom& polynom::operator+=(vector<bool> vec) {
	if (is_zero()) {
		n = vec.size();
		data = vector<bool>(pow(2,n));
	}
	else if (n != vec.size())
		throw "Number of variables are different in polynom& operator+=(vector<bool>)";
	data[vec_to_int(vec)].flip();
	make_poly();
	return *this;
}

//adds vector of monoms to *this
polynom& polynom::operator+=(const vector<vector<bool>>& vec) {
	for (int i = 0; i < vec.size(); ++i) {
		*this += vec[i];
	}
	return *this;
}

template <typename TYPE>
polynom polynom::operator+(TYPE t) const {
	polynom rez = *this;
	return rez += t;
}

bool polynom::operator==(const polynom& p) const  {
	if (is_zero() && p.is_zero())
		return true;
	if (data != p.data) 
		return false;
	return true;
}

#endif