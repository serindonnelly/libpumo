#pragma once
#include "vecN.h"

class ONBasis
{
public:
	bool setA(const vec& v);

	bool setB(const vec& v);

	bool setC(const vec& v);

	ONBasis(vec x, vec y, vec z);

	vec rebase(const vec& v) const;


private:
	vecN a; bool aSet;
	vecN b; bool bSet;
	vecN c; bool cSet;
};