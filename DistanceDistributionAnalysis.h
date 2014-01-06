#pragma once
#include <map>
#include <list>
#include "JointDistributionAnalysis.h"
class DistanceDistributionAnalysis :
	public JointDistributionAnalysis
{
public:
	DistanceDistributionAnalysis();
	~DistanceDistributionAnalysis();
private:
	virtual void updateImpl();
	virtual bool selectDistribution(const Node* n, int& selection) const;
	virtual void collectSample(
		std::vector<float>& samplesX,
		std::vector<float>& samplesY,
		std::vector<float>& sampleWeights) const;
	std::map<int, std::list<float>> distances;
	std::vector<float> binTotals;
	void annotateDistances(const Forest* f);
};

