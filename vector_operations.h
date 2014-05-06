#ifndef VECTOR_OPERATIONS_HPP
#define VECTOR_OPERATIONS_HPP

#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

//display vector<bool>
ostream& operator<<(ostream& out, const vector<bool>& pol);

//add elements of v2 to the end of v1
vector<bool>& operator+=(vector<bool>& v1, const vector<bool>& v2);

//return vector with elements of v1 += v2
vector<bool> operator+(const vector<bool>& v1, const vector<bool>& v2);

//bitwise or of two vector<bool> and put result to v1
vector<bool>& operator|=(vector<bool>& v1, const vector<bool>& v2);

//bitwise or of two vector<bool>
vector<bool> operator|(const vector<bool>& v1, const vector<bool>& v2);

//bitwise end of two vector<bool>
vector<bool> operator&(const vector<bool>& v1, const vector<bool>& v2);

//bitwise xor of two vector<bool>
vector<bool> operator^(const vector<bool>& v1, const vector<bool>& v2);

//bitwise xor of two vector<bool>
vector<bool> operator^=(vector<bool>& v1, const vector<bool>& v2);

//calculating n!
unsigned long int factorial(int n);

//binary representation of x with vector<bool> with num numbers
vector<bool> int_to_vec(int x, int num);

//integer representation of vector<bool> vec
int vec_to_int(const vector<bool> & vec);

//computes the binary logarithm of x
int _log2( unsigned int x );

#endif
