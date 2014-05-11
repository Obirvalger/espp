#ifndef MULTI_AFFINE_HPP
#define MULTI_AFFINE_HPP

#include <iostream>
#include <cmath>
#include <vector>

#include "multi_affine.h"
#include "vector_operations.hpp"
#include "polynom.hpp"

using namespace std;

//constructs data with vec
multi_affine::multi_affine(const vector<vector<bool>> &vec) {
	if (vec.empty())
		throw "Empty initializer in multiaffine(const vector<vector<bool>>&)";
	n = vec[0].size() - 1;
	for (int i = 0; i < vec.size(); ++i) {
		if (n != vec[i].size()) {
			throw "Number of variables are different in \
			multiaffine(const vector<vector<bool>>&)";
		}
		if (vec[i] == vector<bool>(n + 1)) {
			data.clear();
			break;
		}
		if ((find(vec[i]) != -1) || \
		(vec[i] == (vector<bool>(1,1) + vector<bool>(n))) && \
		(!data.empty() || (i != vec.size() - 2))) {
			continue;
		}
		data.push_back(vec[i]);
	}
}

int multi_affine::find(vector<bool> func) const {
	for(int i = 0; i < data.size(); ++i) {
		if (data[i] == func) {
			return i;
		}
	}
	return -1;
}

//multiply this to func(add func to data)
multi_affine& multi_affine::operator*=(vector<bool> func) {
	//multiplication to 1 do nothing
	if (!(func.empty() || data.empty() || func == (vector<bool>(1,1) + vector<bool>(n)))) {
		if (func == vector<bool>(func.size())) {
			data.clear();
		} else if ((data.size() == 1) && (data[0] == (vector<bool>(1,1) + vector<bool>(n)))) {
			data[0] = func;
		} else if (find(func) == -1) {
			data.push_back(func);
		}
	}

	return *this;
}

//if type == 1 construct affine function else construct product of xi if vec[i] == 1
multi_affine::multi_affine(const vector<bool>& vec,unsigned int type) {
	if (type > 1)
		throw "Bad type value in multi_affine(const vector<bool>& vec,unsigned int type)";
	if ((vec != vector<bool>(vec.size()) && (vec.size() != 0))) {
		if (type == 0) {
			n = vec.size();
			vector<bool> v(n + 1);
			
			if (v == vector<bool>(n)) {
				v[0] = 1;
				data.push_back(v);
				return;
			}
				
			for (int i = 0; i < n; ++i) {
				if (vec[i]) {
					v[i + 1] = 1;
					data.push_back(v);
					v = vector<bool>(n + 1);
				}
			}
		} else {
			n = vec.size() - 1;
			data.push_back(vec);
		}
	}
}

//print multi_affine to out
template<typename T>
T& operator<<(T& out, const multi_affine& m) {
	if (m.data.empty()) {
		out<<'0';
		return out;
	}
	bool first = true;
	for (int i = 0; i < m.data.size(); ++i) {
		first = true;
		if (m.data.size() != 1)
			out<<'(';
		for (int j = 1; j <= m.n; ++j) {
			if (m.data[i][j]) {
				if (!first)
					out<<" + ";
				else {
					first = false;
				}
				out<<'x'<<j;
			}
		}
		if (m.data[i][0]) {
			if (!first)
				out<<" + ";
			if ((i != 0) && first) {
				continue;
			}
			out<<1;
		}
		if (m.data.size() != 1)
			out<<')';	
	}
	return out;
}

//make polynomial representarion of multi affine function(multiply all linear functions)
multi_affine::operator polynom() const {
	if (data.empty())
		return polynom();
	polynom p(data[0], 2);
	for (int i = 1; i < data.size(); ++i) {
		p *= polynom(data[i], 2);
	}
	return p;
}

#endif