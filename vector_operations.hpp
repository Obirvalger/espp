#ifndef VECTOR_OPERATIONS_HPP
#define VECTOR_OPERATIONS_HPP

#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

//display vector<bool>
ostream& operator<<(ostream& out, const vector<bool>& pol) {
	for(int i = 0; i < pol.size(); ++i) {
		out<<pol[i]<<' ';
	}
	out<<endl;
	return out;
}

//add elements of v2 to the end of v1
vector<bool>& operator+=(vector<bool>& v1, const vector<bool>& v2) {
	v1.insert(v1.end(), v2.begin(), v2.end());
	return v1;		
}

//return vector with elements of v1 += v2
vector<bool> operator+(const vector<bool>& v1, const vector<bool>& v2) {
	vector<bool> v = v1;
	return v += v2;		
}

//bitwise or of two vector<bool> and put result to v1
vector<bool>& operator|=(vector<bool>& v1, const vector<bool>& v2) {
	if (v1.size() != v2.size())
		throw "operator| vector<bool> parametrs have different size";
	for (int i = 0; i < v1.size(); ++i) {
		v1[i] = v1[i] | v2[i];
	}
	return v1;		
}

//bitwise or of two vector<bool>
vector<bool> operator|(const vector<bool>& v1, const vector<bool>& v2) {
	vector<bool> rez = v1;
	return rez |= v2;
}

//bitwise end of two vector<bool>
vector<bool> operator&(const vector<bool>& v1, const vector<bool>& v2) {
	if (v1.size() != v2.size())
		throw "operator& vector<bool> parametrs have different size";
	vector<bool> vec(v1.size());
	for (int i = 0; i < v1.size(); ++i) {
		vec[i] = v1[i] & v2[i];
	}
	return vec;		
}

//bitwise xor of two vector<bool>
vector<bool> operator^(const vector<bool>& v1, const vector<bool>& v2) {
	if (v1.size() != v2.size())
		throw "operator^ vector<bool> parametrs have different size";
	vector<bool> vec(v1.size());
	for (int i = 0; i < v1.size(); ++i) {
		vec[i] = v1[i] ^ v2[i];
	}
	return vec;		
}

//bitwise xor of two vector<bool>
vector<bool> operator^=(vector<bool>& v1, const vector<bool>& v2) {
    //cout<<"v1 = "<<v1<<"v2 = "<<v2;
	if (v1.size() != v2.size())
        throw "operator^= vector<bool> parametrs have different size";
	for (int i = 0; i < v1.size(); ++i) {
		v1[i] = v1[i] ^ v2[i];
	}
	return v1;		
}

//calculating n!
unsigned long int factorial(int n) {
	int res = 1;
	for (int i = 2; i <= n; i++) {
		res *= i;
	}
	return res;
}

//binary representation of x with vector<bool> with num numbers
vector<bool> int_to_vec(int x, int num) {
	vector<bool> v(num);
	for(int i = 0; i < num; ++i) {
		v[num - 1 - i] = x % 2;
		x /= 2;
	}
	return v;
}

//integer representation of vector<bool> vec
int vec_to_int(const vector<bool> & vec) {
	int ans = 0, sz = vec.size();
	for(int i = 0; i < sz; ++i) {
		if (vec[sz - 1 - i]) {
			ans += pow(2, i);
		}
	}
	return ans;
}

//computes the binary logarithm of x
int _log2( unsigned int x )
{
  unsigned int ans = 0, y = x;
  while( x>>=1 ) ans++;
  if((y - pow(2, ans)) != 0)
  	return -1;
  return ans ;
}

#endif
