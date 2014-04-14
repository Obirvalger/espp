#include "course.hpp"

using namespace std;

int main() 
try {
	const int n = 2;
	ofstream out("equal_classes.txt");
	
	vector<equal_functions> eq_classes = all_eq_classes(n, make_all_changes(n));
	for (int i = 0; i < eq_classes.size(); ++i) {
		cout<<eq_classes[i];
	}
	cout<<"Number of equivalence classes = "<<eq_classes.size()<<endl;
	
	out<<"There is ";
	out<<eq_classes.size();
	out<<" equivalence classes\n\n";
	out<<eq_classes;
} 
catch(const char* str) {
	cout<<str<<endl;
}