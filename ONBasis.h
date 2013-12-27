#pragma once
#include "vecN.h"

class ONBasis
{
public:
	ONBasis() : a(1.f, 0.f, 0.f), b(0.f, 1.f, 0.f), c(0.f, 0.f, 1.f) {};

	bool setA(const vec& v);

	bool setB(const vec& v);

	bool setC(const vec& v);

	ONBasis(vec x, vec y, vec z);

	vec rebase(const vec& v) const;

	bool valid() { return (aSet && bSet && cSet) || (!aSet && !bSet && !cSet); }

private:
	vecN a; bool aSet;
	vecN b; bool bSet;
	vecN c; bool cSet;
};