#ifndef AFFINE_CHANGES_HPP
#define AFFINE_CHANGES_HPP

#include "affine_changes.h"

using namespace std;

vector<vector<bool>> reverse(vector<vector<bool>> matrix) {
	int i, j, k, l, n = matrix.size();
	vector<bool> tmp;
	vector<vector<bool>> reverse(n, vector<bool>(n + 1));
	bool f;
	
	for (int i = 0; i < n; ++i) {
		reverse[i][i + 1] = 1;
		reverse[i][0] = matrix[i][0];
	}
	
	for (i = 0; i < n - 1; ++i) {
		if (!matrix[i][i + 1]) {
			f = false;
			for (l = i; l < n; ++l) {
				if (matrix[l][i + 1]) {
					tmp = matrix[l];
					matrix[l] = matrix[i];
					matrix[i] = tmp;
					
					tmp = reverse[l];
					reverse[l] =reverse[i];
					reverse[i] = tmp;
					
					f = true;
				}
			}
			if (!f)
				throw "Reversing degenerate matrix!";
		}
		for (j = i + 1; j < n; ++j) {
			if (matrix[j][i + 1]) {
				matrix[j] ^= matrix[i];
				reverse[j] ^= reverse[i];
			}
		}
	}
	
	if (!matrix[n-1][n]) 
				throw "Reversing degenerate matrix!";
	
	if (n == 1)
		return reverse;
	
	for (i = 0; i < n - 1; ++i) {
		for (j = n - i; j < n + 1; ++j) {
			if (matrix[n - 2 - i][j]) {
				reverse[n - 2 - i] ^= reverse[j - 1];
			}
		}	
	} 
	
	return reverse;
}

//vec[i] is change of xi
affine_change::affine_change(const vector<vector<bool>>& vec) {
	if (vec.empty())
		throw "Empty initializer in affine_change(const vector<vector<bool>>&)";
	n = vec[0].size() - 1;
	int i = 0;
	for (; i < vec.size(); ++i) {
		if (n != vec[i].size() - 1)
			throw "Number of variables are different in affine_change(const vector<vector<bool>>&)";
		data.push_back(polynom(vec[i],2));
	}
	if (i != n - 1) {
		vector<bool> v(n + 1);
		for (; i < n; ++i) {
			v[i + 1] = 1;
			data.push_back(polynom(v,2));
			v[i + 1] = 0;
		}
	}
}

vector<vector<bool>> affine_change::get_matrix() const {
	vector<vector<bool>> matrix(n, vector<bool>(n + 1));
	vector<bool> tmp;
	unsigned int p = 1;
	
	for (unsigned int i = 0; i < n; ++i) {
		tmp = data[i].get_data();
		matrix[i][0] = tmp[0];
		p = 1;
		for (unsigned int j = 0; j < n; ++j) {
			matrix[i][n - j] = tmp[p];
			p *= 2;
		}	
	} 
	
	return matrix;
}


affine_change affine_change::reverse() const {
	return ::reverse(this->get_matrix());
}

void affine_change::set_negations(vector<bool> negations) {
	if (negations.size() != n)
		throw "Wrong number of variables in affine_change::set_negations(vector<bool>)";
	vector<bool> one(n);
	for (int i = 0; i < n; ++i) {
		if (negations[i]) {
			if (data[i].find(one) == -1) {//if there is not "1" in polynomial data[i]
				data[i] += one;	
			}	
		} else {
			if (data[i].find(one) != -1) {//if there is "1" in polynomial data[i]
				data[i] += one;
			}	
		}
	}
}

void affine_change::set_order(const vector<int>& order) {
	if (order.size() != n)
		throw "Wrong number of variables in affine_change::set_order(const vector<int>&)";
	vector<polynom> tmp;
	for (int i = 0; i < n; ++i) {
		tmp.push_back(data[order[i]]);	
	}
	data = tmp;
}

ostream& operator<<(ostream& out, const affine_change& a) {
	for (int i = 0; i < a.data.size(); ++i) {
		out<<'x'<<i + 1<<" = "<<a.data[i];
	}
	return out;
}

bool nondegenerate(vector<vector<bool>> matrix) {
	int i, j, k, l, n = matrix.size();
	vector<bool> tmp;
	bool f;
	for (i = 0; i < n - 1; ++i) {
		if (!matrix[i][i + 1]) {
			f = false;
			for (l = i; l < n; ++l) {
				if (matrix[l][i + 1]) {
					tmp = matrix[l];
					matrix[l] = matrix[i];
					matrix[i] = tmp;
					f = true;
				}
			}
			if (!f) 
				return f;
		}
		for (j = i + 1; j < n; ++j) {
			if (matrix[j][i + 1])
				matrix[j] ^= matrix[i];
		}
	}
	return matrix[n-1][n];
}

vector<affine_change> nondegenerate_linear_changes(int n) {
	vector<affine_change> var;
	vector<vector<bool>> vec(n,vector<bool>(n + 1));
	unsigned long int pow_2_n = pow(2,n);
	int k;
	for (int i = 0; i < pow(2,n*n); ++i) {
		k = i;
		for (int j = 0; j < n; ++j) {
			vec[n - 1 - j] = int_to_vec(k % pow_2_n, n + 1);
			k /= pow_2_n;
		}
		if (nondegenerate(vec))
			var.push_back(vec);	
	}
	return var;
}

vector<affine_change> make_all_changes(int n) {
	vector<affine_change> tmp = nondegenerate_linear_changes(n), var = tmp;
	vector<bool> negations;
	for (int i = 1; i < pow(2,n); ++i) {
		negations = int_to_vec(i,n);
		for (int j = 0; j < tmp.size(); ++j) {
			tmp[j].set_negations(negations);
		}
		var.insert(var.end(), tmp.begin(), tmp.end());
	}
	
	return var;
}

polynom polynom::change_variables(const affine_change& var) const {
	polynom p, tmp;
	for (int i = 0; i < poly.size(); ++i) {
		for (int j = 0; j < poly[i].size(); ++j) {
			if (poly[i][j]) {
				if (tmp.is_zero()) {
					tmp += var[j];
				} else {
					tmp *= var[j];
				}
			}
		}
		p += tmp;
		tmp.clear();
	}
	if (data[0])
		p += polynom((vector<bool>(1,1) + vector<bool>(pow(2,n) - 1)), 1);
	return p;
}

void save_changes(const vector<affine_change>& changes, const char* str) {
	ofstream out(str);
	vector<vector<bool>> matrix;
	for (auto& x : changes) {
		matrix = x.get_matrix();
		for (unsigned int i = 0; i < matrix.size(); ++i) {
			for (unsigned int j = 0; j < matrix[i].size(); ++j) {
				out<<matrix[i][j];	
			}
			out<<',';
		}
	}
}

vector<affine_change> load_changes(const char* str) {
	ifstream in(str);
	char c = in.get();
	vector<affine_change> changes;
	vector<vector<bool>> matrix;
	vector<bool> vec;
	unsigned int n, i, j;
	
	while (in && (c != ',')) {
		vec.push_back(c - '0');
		c = in.get();
	}
	
	matrix.push_back(vec);
	n = vec.size() - 1;
	i = 1;
	c = in.get();
	
	while ((i < n)) {
		while ((c != ',')) {
			vec[j++] = (c - '0');
			c = in.get();
		}
		
		matrix.push_back(vec);
		c = in.get();
		++i;
		j = 0;
	}
	
	changes.push_back(matrix);
	
	i = 0;
	
	while (in) {
		while ((i < n)) {
			while ((c != ',')) {
				vec[j++] = (c - '0');
				c = in.get();
			}
			
			matrix[i++] = (vec);
			c = in.get();
			//++i;
			j = 0;
		}
		
		i = 0;
		changes.push_back(matrix);
	}
	
	return changes;
}

#endif