#pragma once
#include "Analysis.h"
class MonteCarloAnalysis :
	public Analysis
{
public:
	MonteCarloAnalysis();
	~MonteCarloAnalysis();
	virtual void load();
	std::vector<float> widths;
private:
	virtual void updateImpl();
	virtual void save();
	int mTreeCount = 3;
};

