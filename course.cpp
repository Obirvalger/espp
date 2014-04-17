#include "course.hpp"

using namespace std;

int main() 
try {
	const int n = 3;
	//polynom p = vector<vector<bool>>{{1,1}};
	ofstream out("equal_classes.txt");
	vector<affine_change> all_changes = make_all_changes(n);
	/*for (auto& x : all_changes)
		cout<<x<<endl;*/
	//cout<<p<<all_changes[1]<<p.change_variables(all_changes[1])<<endl;
	vector<equal_functions> eq_classes = all_eq_classes(n, all_changes);
	out<<"There is ";
	out<<eq_classes.size();
	out<<" equivalence classes\n\n";
	print(cout, eq_classes, all_changes);
	print(out, eq_classes, all_changes);
	cout<<"Number of equivalence classes = "<<eq_classes.size()<<endl;
	
	make_stat(eq_classes);
	
	/*out<<"There is ";
	out<<eq_classes.size();
	out<<" equivalence classes\n\n";
	out<<eq_classes;*/
} 
catch(const char* str) {
	cout<<str<<endl;
}