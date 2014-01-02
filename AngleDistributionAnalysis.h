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
	virtual void updateImpl();
	virtual bool generateAngle(const Node* n, float& newAngle) const;
private:
	virtual bool serialise(picojson::value& v) const;
	virtual bool deserialise(const picojson::value& v);
	void zero();
	std::piecewise_constant_distribution<float> mDistribution;
};

