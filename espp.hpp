#ifndef ESPP_HPP
#define ESPP_HPP

#include <iostream>
#include <cmath>
#include <vector>

#include "espp.h"
#include "vector_operations.hpp"
#include "polynom.hpp"
#include "multi_affine.hpp"

using namespace std;

int espp::find(const multi_affine& m) {
		for(int i = 0; i < data.size(); ++i) {
			if (data[i] == m) {
				return i;
			}
		}
		return -1;
}

espp& espp::operator+=(const multi_affine& m) {
		if (empty()) {
			n = m.get_n();
			data.push_back(m);
			return *this;
		}
		
		/*if (m.is_one()) {
			cout<<*this;
		}*/
		
		int i;
		if ((i = find(m)) == -1)
			data.push_back(m);
		else
			data.erase(data.begin() + i);
			
		/*if (m.is_one()) {
			cout<<*this<<"\n";
		}*/
		
		return *this;
}
	
template <typename TYPE>
espp espp::operator+(const TYPE m) const{
		espp e = *this;
		return e += m;
}

template <typename T>
T& operator<<(T& out, const espp& e) {
	if (e.data.empty()) {
		out<<'0';
		return out;
	}
	bool first = true;
	for (int i = 0; i < e.data.size(); ++i) {
		if (!first)
			out<<" + ";
		else first = false;
		out<<e.data[i];
	}
	out<<endl;
	return out;
}

espp& espp::operator+=(const espp& e) {
	
	if (e.empty()) {
		return *this;	
	}
	
	if (empty()) {
		*this = e;
		return *this;	
	}
	
	for (int i = 0; i < e.data.size(); ++i) {
		if (find(e.data[i]) == -1)
			data.push_back(e.data[i]);
		else
			data.erase(data.begin() + i);	
	}
	return *this;
}

#endif