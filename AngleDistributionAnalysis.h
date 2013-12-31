#pragma once
#include <random>
#include "DistributionAnalysis.h"
class AngleDistributionAnalysis :
	public DistributionAnalysis
{
public:
	AngleDistributionAnalysis();
	~AngleDistributionAnalysis();
	int binCount;
	int sampleCount;
	std::vector<float> binBoundaries;
	std::vector<float> binWeights;
	virtual void load();
	virtual void updateImpl();
	virtual bool generateAngle(const Node* n, float& newAngle) const;
private:
	virtual void save();
	virtual void zero();
	std::piecewise_constant_distribution<float> mDistribution;
};

