#include "data_structures.hpp"
using namespace std;

//comparing two vector<bool> to less or equal
bool operator<=(const vector<bool>& v1, const vector<bool>& v2) {
	for (int i = 0; i < v1.size(); ++i) {
		if (v1[i] > v2[i])
			return false;
	}
	return true;
}

//comparing two vector<bool> to less or equal
bool operator>=(const vector<bool>& v1, const vector<bool>& v2) {
	for (int i = 0; i < v1.size(); ++i) {
		if (v1[i] < v2[i])
			return false;
	}
	return true;
}

//computes down shadow of alpha: all vectors <= alpha and it has length = |alpha| - 1
vector<vector<bool>> down_shadow(vector<bool> alpha) {
	vector<vector<bool>> vec;
	vector<bool> a;
	for (int i = 0; i < alpha.size(); ++i) {
		if (alpha[i]) {
			a = alpha;
			a[i].flip();
			vec.push_back(a);
		}
	}
	return vec;
}

//computes down shadow of alpha: all vectors >= alpha and it has length = |alpha| + 1
vector<vector<bool>> up_shadow(vector<bool> alpha) {
	vector<vector<bool>> vec;
	vector<bool> a;
	for (int i = 0; i < alpha.size(); ++i) {
		if (!alpha[i]) {
			a = alpha;
			a[i].flip();
			vec.push_back(a);
		}
	}
	return vec;
}

//computes ball radius of the one with the center at the point alpha
vector<vector<bool>> ball1(vector<bool> alpha) {
	vector<vector<bool>> vec;
	vector<bool> a;
	
	for (int i = 0; i < alpha.size(); ++i) {
		if (alpha[i]) {
			a = alpha;
			a[i].flip();
			vec.push_back(a);
		}
	}
	
	vec.push_back(alpha);
	
	for (int i = 0; i < alpha.size(); ++i) {
		if (!alpha[i]) {
			a = alpha;
			a[i].flip();
			vec.push_back(a);
		}
	}
	return vec;
}

//computes shading set of n demensional boolean cube 
vector<vector<bool>> make_shadow(int n) {
	if (n == 3) {
		return vector<vector<bool>>{{0,0,0}, {1,1,1}};
	}
	if (n == 4) {
		return vector<vector<bool>>{{0,0,0,0}, {1,0,0,0}, {0,1,1,1}, {1,1,1,1}};
	}
	//else ...	
}

polynom polynom::make_pj(vector<bool> alpha) const {
	if (data.empty())
		return polynom();
	vector<vector<bool>> ball = ball1(alpha);
	polynom p;
	for (int i = 0; i < ball.size(); ++i) {
		if (data[vec_to_int(ball[i])])
			p += ball[i];
	}
	p.make_poly();
	return p;
}

polynom polynom::make_pj_up(vector<bool> alpha) const {
	if (data.empty())
		return polynom();
	vector<vector<bool>> up = up_shadow(alpha);
	polynom p;
	for (int i = 0; i < up.size(); ++i) {
		if (data[vec_to_int(up[i])])
			p += up[i];
	}
	p.make_poly();
	return p;
}

//rule of conversion polynom to espp	
espp rule1(const polynom& p, const vector<bool>& alpha, bool with_alpha = true) {
	if (p.get_n() != alpha.size())
		throw "Number of variables are different in rule1";
	espp e;
	vector<vector<bool>> up = up_shadow(alpha);
	if (with_alpha) up.push_back(alpha);
	vector<bool> vec(alpha.size()), v(alpha.size() + 1);
	for (int i = 0; i < up.size(); ++i) {
		if (p.test(up[i])) {
			if (alpha == up[i])
				v[0] = 1;
			else
				vec |= (alpha ^ up[i]);
		}
	}
	for (int i = 0; i < vec.size(); ++i) {
		v[i + 1] = vec[i];
	}
	if (!with_alpha) v[0].flip();
	e += multi_affine(alpha,0) *= v;
	return e;	
}

//main algorithm 
espp::espp(vector<bool> vec, bool type) {
	polynom p(vec, type), pj, pj_up, pj_down, p_rest;
	espp q, y_up, y_down;
	vector<vector<bool>> t = make_shadow(p.get_n());
	for (int j = 0; j < t.size(); ++j) {
		pj = p.make_pj(t[j]);
		pj_up = p.make_pj_up(t[j]);	
		if (!p.test(t[j]))
			pj_up += t[j];
		pj_down = pj + pj_up;
		y_up = rule1(pj_up, t[j]);
		cout<<"p = "<<p<<"j = "<<j<<" t[j] = "<<t[j]<<"pj = "<<pj<<"pj_up = "<<pj_up\
		<<"y_up = "<<y_up<<"\n\n";
		
		
		p += (pj + p_rest);
		q += (y_up + y_down);
	}
}

int main() 
try {
	vector<bool> v1{1,0,0,1,1,0,1,1}, v2{0,0,0,0,0,0,1,1}, v3{1,0,1,0,1}, \
	v4{0,1,1,0,0}, v5{0,1,0}, v0{0,0,0,0,0};
	polynom p0, p1(v1,1), p2(v2,0), p3(v3,2), p4(v4,2), \
	p5(vector<vector<bool>>{{0,1,1,0},{0,0,1,0},{0,0,0,1},{0,0,0,0}}), \
	p6(vector<vector<bool>>{{1,1},{0,0},{1,0},{0,1}});
	multi_affine m1(), m2(v3,1);
	//affine_change a(vector<vector<bool>>{{1,1,1},{1,0,1}});
	//vector<affine_change> vc = make_all_changes(2);
	/*for (int i = 0; i < vc.size(); ++i) {
		cout<<vc[i]<<endl;
	}
	cout<<vc.size();*/
	espp e(p5.get_data(), 1);
	(m1 *= v3) *= v4;
	(e += m1) += m1;
	e = rule1(p1, v5);
	/*p5 += p5;
	cout<<p5;*/
	//m2 *= vector<bool>{1,0,0,0,0};
	//m2 *= vector<bool>{0,1,0,0,0};
	cout<<e<<endl;
	//cout<<p0<<(p0 += polynom(vector<bool>{0,1,0},2));
	//cout<<p6<<a<<p6.change_variables(a);
	//cout<<p1.get_data()<<p1<<endl<<p2.get_data()<<p2<<endl<<(p1 * p2)<<endl;
	//cout<<p3<<p4<<(p3 *= p4)<<p1<<p2<<p1*p2;
} 
catch(const char* str) {
	cout<<str<<endl;
}