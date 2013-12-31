#pragma once
#include "Analysis.h"
#include "Node.h"
class DistributionAnalysis : public Analysis
{
public:
	DistributionAnalysis();
	~DistributionAnalysis();
	virtual bool generateAngle(const Node* n, float& newAngle) const = 0;
private:
	virtual void zero() = 0;
};

