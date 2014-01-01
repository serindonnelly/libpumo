#pragma once
#include "JointDistributionAnalysis.h"
class HeightDistributionAnalysis :
	public JointDistributionAnalysis
{
public:
	HeightDistributionAnalysis();
	~HeightDistributionAnalysis();
private:
	virtual bool selectDistribution(const Node* n, int& selection) const;
	virtual void collectSample(
		std::vector<float>& samplesX,
		std::vector<float>& samplesY,
		std::vector<float>& sampleWeights) const;
};

