#pragma once
#include "Analysis.h"
#include "Node.h"
class DistributionAnalysis : public Analysis
{
public:
	DistributionAnalysis();
	~DistributionAnalysis();
	virtual float generateAngle(const Node* n) const = 0;
private:
	virtual void zero() = 0;
};

