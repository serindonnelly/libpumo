#pragma once
#include "vecN.h"
#include "Forest.h"
class Forest;

class Axis
{
public:
	Axis(Forest* f);
	~Axis();
	vecN getAxis();
	const static int numSamples;
protected:
	vecN mAxis;
	bool calculated;
	Forest* mForest;

	virtual void calculateAxis() =0;
};

class PFAxis : public Axis
{
public:
	PFAxis(Forest* f) : Axis(f) {};
protected:
	virtual void calculateAxis();
};

class LongAxis : public Axis
{
public:
	LongAxis(Forest* f) : Axis(f) {};
protected:
	virtual void calculateAxis();
};

class SideAxis : public Axis
{
public:
	SideAxis(Forest* f) : Axis(f) {};
protected:
	virtual void calculateAxis();
};