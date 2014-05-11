#ifndef PRAC_HPP
#define PRAC_HPP

#include "vector_operations.hpp"
#include "polynom.hpp"
#include "multi_affine.hpp"
#include "espp.hpp"

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
	if (n == 2) {
		return vector<vector<bool>>{{0,0}, {1,1}};
	}
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
		//cout<<"ball[i] = "<<ball[i]<<"\n";
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
	
	if (p.is_zero())
		return espp();
	
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

//later will be implement rule2, maybe it returns pair(espp,polynom)

//main algorithm 
espp::espp(vector<bool> vec, bool type) {
	polynom p(vec, type), pj, pj_up, pj_down, p_rest;
	espp q, y_up, y_down;
	vector<vector<bool>> t = make_shadow(p.get_n());
	for (int j = 0; j < t.size(); ++j) {
		
		//cout<<"j = "<<j<<"\n";
		
		pj = p.make_pj(t[j]);
		pj_up = p.make_pj_up(t[j]);	
		if (!p.test(t[j]))
			pj_up += t[j];
		pj_down = pj + pj_up;
		cout<<"p = "<<p<<"pj = "<<pj<<"pj_up = "<<pj_up;
		y_up = rule1(pj_up, t[j], 0);
		cout<<"y_up = "<<y_up<<endl;
		/*cout<<"p = "<<p<<"j = "<<j<<" t[j] = "<<t[j]<<"pj = "<<pj<<"pj_up = "<<pj_up\
		<<"y_up = "<<y_up<<"\n\n";*/
		
		//using rule2, it gives us y_down and p_rest
		p += (pj + p_rest);
		q += (y_up + y_down);
	}
}
#endif