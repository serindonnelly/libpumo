#pragma once
#include "vec.h"
struct vecN :
	public vec
{
public:
	explicit vecN(const vec& v) :vec(v / v.norm()) { ; }
	vecN(float x, float y, float z) : vecN(vec(x, y, z)) { ; }
	vec project(const vec& p) const
	{
		return dot(p) * v();
	}
	vec projectOrth(const vec& p) const
	{
		return p - project(p);
	}
	float cosine(const vec& p) const
	{
		return dot(p) / p.norm();
	}
	float cosine(const vecN& p) const
	{
		return dot(p);
	}
};
