#include "data_structures.hpp"

int main() 
try {
	vector<bool> v1{1,0,0,1,1,0,1,1}, v2{0,0,0,0,0,0,1,1}, v3{1,0,1,0,1}, v4{0,1,1,0,0};
	polynom p0, p1(v1,1), p2(v2,0), p3(v3,2), p4(v4,2), p6(vector<vector<bool>>{{1,1},{0,0},{1,0},{0,1}});
	multi_affine m1(v3.size() - 1), m2(v3,1);
	espp e;
	(m1 *= v3) *= v4;
	(e += m1) += m2;
	cout<<"m1 = "<<m1<<"\nm2 = "<<m2<<"\ne = "<<e<<endl;
	cout<<"p0 = "<<p0<<"p1 = "<<p1<<"p2 = "<<p2<<"p3 = "<<p3<<"p4 = "<<p4<<"p6 = "<<p6<<endl;
	cout<<"p3 *= p4 = "<<(p3 *= p4)<<"p1 * p2 = "<<p1 * p2<<"p0 + p1 + p2 = "<<p0+p1+p2;
} 
catch(const char* str) {
	cout<<str<<endl;
}