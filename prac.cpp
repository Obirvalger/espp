#include "prac.hpp"

using namespace std;

int main() 
try {
	vector<bool> vec = {1,0,0,0};//getting vector of function (later from file, cin or cmd args)
	espp e(vec,0);
	cout<<e;
} 
catch(const char* str) {
	cout<<str<<endl;
}