#ifndef ESPP_HPP
#define ESPP_HPP

#include <iostream>
#include <cmath>
#include <vector>

#include "espp.h"

using namespace std;

int espp::find(const multi_affine& m) {
		for(int i = 0; i < data.size(); ++i) {
			if (data[i] == m) {
				return i;
			}
		}
		return -1;
}

espp::espp(const multi_affine& m) {
    n = m.get_n();
    data.push_back(m);
}

espp& espp::operator+=(const multi_affine& m) {
		if (empty()) {
			n = m.get_n();
			data.push_back(m);
			return *this;
		}
		
		/*if (m.is_one()) {
			cout<<*this;
		}*/
		
		int i;
		if ((i = find(m)) == -1)
			data.push_back(m);
		else
			data.erase(data.begin() + i);
			
		/*if (m.is_one()) {
			cout<<*this<<"\n";
		}*/
		
		return *this;
}
	
template <typename TYPE>
espp espp::operator+(const TYPE m) const{
		espp e = *this;
		return e += m;
}

template <typename T>
T& operator<<(T& out, const espp& e) {
	if (e.data.empty()) {
		out<<'0';
		return out;
	}
	bool first = true;
	for (int i = 0; i < e.data.size(); ++i) {
		if (!first)
			out<<" + ";
		else first = false;
		out<<e.data[i];
	}
	out<<endl;
	return out;
}

espp& espp::operator+=(const espp& e) {
	
	if (e.empty()) {
		return *this;	
	}
	
	if (empty()) {
		*this = e;
		return *this;	
	}
	
	for (int i = 0; i < e.data.size(); ++i) {
		if (find(e.data[i]) == -1)
			data.push_back(e.data[i]);
		else
			data.erase(data.begin() + i);	
	}
    
    /*vector<bool> v(n + 1, 0), u = v;
	for (int i = 0; i < data.size(); ++i) {
		if (data[i].is_affine())
			(u = data[i].get_data()[0]);
            v ^= u;
            //cout<<(v ^= vector<bool>{0,0,1});
            cout<<"v = "<<(v);// ^ data[i].get_data()[0]); 
            cout<<"data = "<<data[i].get_data()[0];
            cout<<" v + data = "<<(v ^ data[i].get_data()[0]);
			data.erase(data.begin() + i);	
	}

    if (v != vector<bool>(v.size())) {
        cout<<"v = "<<v; 
        data.push_back(multi_affine(v, 0));
    }*/

	return *this;
}

espp::operator polynom() const {
	if (data.empty())
		return polynom();
	polynom p = data[0];
	for (int i = 1; i < data.size(); ++i) {
		p += data[i];
	}
	return p;
}

#endif
