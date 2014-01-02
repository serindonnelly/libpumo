#pragma once
#include "JointDistributionAnalysis.h"
class ParentDistributionAnalysis :
	public JointDistributionAnalysis
{
public:
	ParentDistributionAnalysis();
	~ParentDistributionAnalysis();
private:
	virtual bool selectDistribution(const Node* n, int& selection) const;
	virtual void collectSample(
		std::vector<float>& samplesX,
		std::vector<float>& samplesY,
		std::vector<float>& sampleWeights) const;
	virtual bool preferredBinBoundariesX(float& minX, float& maxX)
	{
		minX = 0.f; maxX = 180.f;
		return true;
	}
	virtual bool preferredBinBoundariesY(float& minY, float& maxY)
	{
		minY = 0.f; maxY = 180.f;
		return true;
	}
};

