#include "prac.hpp"

using namespace std;

int main() 
try {
	const int n = 2;
    vector<bool> vec = {1,1,0,0};//getting vector of function
	cout<<"start\n\n";
	
	espp e(vec,0);
    cout<<"p = "<<polynom(vec,0)<<"\ne = "<<e<<"\npolynom(e) = "<<polynom(e)<<endl;
	
    for (int i = 0; i < pow(2,pow(2,n)); ++i) {
        //p = polynom(int_to_vec(i,pow(2,n)), 0); 
        bool b = !(polynom(int_to_vec(i,pow(2,n)), 0) == polynom(espp(int_to_vec(i,pow(2,n)),0)));
        if (b) {
            cout<<polynom(int_to_vec(i,pow(2,n)), 0).get_data()<<\
            polynom(espp(int_to_vec(i,pow(2,n)),0)).get_data()<<\
            polynom(int_to_vec(i,pow(2,n)), 0)<<\
            espp(int_to_vec(i,pow(2,n)),0);
            throw "Very bad!";
        }
    }

	cout<<"All done!\n";
} 
catch(const char* str) {
	cout<<str<<endl;
}
