#include "equivalence_classes.hpp"
//#include "ctime"

using namespace std;

int main() 
try {
	//const int n = 2;
	//polynom p = vector<vector<bool>>{{1,1}};
	ofstream out("equal_classes");
	/*vector<affine_change> all_changes = make_all_changes(n);
	//cout<<p<<all_changes[1]<<p.change_variables(all_changes[1])<<endl;
	vector<equal_functions> eq_classes = all_eq_classes(n, all_changes);
	out<<"There is ";
	out<<eq_classes.size();
	out<<" equivalence classes\n\n";
	//print(cout, eq_classes, all_changes);
	print(out, eq_classes, all_changes);
	cout<<"Number of equivalence classes = "<<eq_classes.size()<<endl;
	make_summary(eq_classes, "summary_2.txt");*/
	
	ifstream file("mrec_4");
	//vector<affine_change> ac = make_all_changes(4);
	//save_changes(ac, "changes_4");
	vector<affine_change> ac = load_changes("changes_4");
	vector<equal_functions> eq = parse_file(file, ac);
	/*make_stat(eq);
	if (ac != ac1)
		cout<<"lol\n";*/
	test(eq,ac);
	make_summary(eq, "summary_4.txt");
	//print(cout, eq, ac);
	//cout<<"begin\n\n";
	//vector<pair<polynom, int>> shorts = shortest(eq, ac);
	
	/*out<<"There is ";
	out<<eq_classes.size();
	out<<" equivalence classes\n\n";
	out<<eq_classes;*/
	
	cout<<"All done!\n";
} 
catch(const char* str) {
	cout<<str<<endl;
}