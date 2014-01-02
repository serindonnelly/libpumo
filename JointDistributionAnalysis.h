#pragma once
#include <random>
#include <vector>
#include "DistributionAnalysis.h"
#include "Histogram2d.h"
class JointDistributionAnalysis :
	public DistributionAnalysis
{
public:
	JointDistributionAnalysis();
	~JointDistributionAnalysis();
	virtual bool generateAngle(const Node* n, float& newAngle) const;
	virtual void updateImpl();
protected:
	Histogram2d mHistogram;
private:
	virtual bool selectDistribution(const Node* n, int& selection) const = 0;
	virtual void collectSample(
		std::vector<float>& samplesX,
		std::vector<float>& samplesY,
		std::vector<float>& sampleWeights) const = 0;
	void updateConditionalDistributions();
	std::vector<std::piecewise_constant_distribution<float>*> mConditionalDistributions;
	virtual bool serialise(picojson::value& v) const;
	virtual bool deserialise(const picojson::value& v);
	virtual int preferredBinCountX() { return 30; }
	virtual int preferredBinCountY() { return 30; }
	virtual bool preferredBinBoundariesX(float& minX, float& maxX) { return false; }
	virtual bool preferredBinBoundariesY(float& minY, float& maxY) { return false; }
};

