#pragma once
#include "Analysis.h"
class AxesAnalysis : public Analysis
{
public:
	AxesAnalysis();
	~AxesAnalysis();
	virtual void load();
private:
	virtual void save();
	virtual void updateImpl();
};

