#include "ONBasis.h"
/***********************************************************************
 *  Method: ONBasis::setA
 *  Params: const vec &v
 * Returns: bool
 * Effects: set 
 ***********************************************************************/
bool
ONBasis::setA(const vec &v)
{
	if (bSet || cSet)
		return false;
	a = vecN(v);
	aSet = true;
	return true;
}

/***********************************************************************
 *  Method: ONBasis::setB
 *  Params: const vec &v
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
ONBasis::setB(const vec &v)
{
	if (cSet || !aSet)
		return false;
	b = vecN(a.projectOrth(v));
	bSet = true;
	return true;
}


/***********************************************************************
 *  Method: ONBasis::setC
 *  Params: const vec &v
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
ONBasis::setC(const vec &v)
{
	if (!bSet || !aSet)
		return false;
	c = vecN(b.projectOrth(a.projectOrth(v)));
	cSet = true;
	return true;
}


/***********************************************************************
 *  Method: ONBasis::ONBasis
 *  Params: vec x, vec y, vec z
 * Effects: 
 ***********************************************************************/
ONBasis::ONBasis(vec x, vec y, vec z) : a(x), b(y), c(z)
{
	setA(x);
	setB(y);
	setC(z);
}


/***********************************************************************
 *  Method: ONBasis::rebase
 *  Params: const vec &v
 * Returns: vec
 * Effects: 
 ***********************************************************************/
vec
ONBasis::rebase(const vec &v) const
{
	return vec(v.dot(a), v.dot(b), v.dot(c));
}


