#pragma once
#include "Analysis.h"
#include "vecN.h"
class AxesAnalysis : public Analysis
{
public:
	AxesAnalysis();
	~AxesAnalysis();
	virtual void load();
	vecN getPF() { return pfAxis; }
private:
	virtual void save();
	virtual void updateImpl();
	vecN pfAxis;
	//vecN longAxis;
	//vecN sideAxis;
};

