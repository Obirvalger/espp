#include "prac.hpp"

using namespace std;

int main() 
try {
	vector<bool> vec = {1,0,0,0};//getting vector of function
	cout<<"start\n";
	
	espp e(vec,0);
	cout<<"e = "<<e<<endl;
	
	cout<<"All done!";
} 
catch(const char* str) {
	cout<<str<<endl;
}