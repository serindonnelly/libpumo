#pragma once
#include "Analysis.h"
#include "MaxDistanceDistributionAnalysis.h"

struct ParentDistanceAngle
{
	ParentDistanceAngle(float p, float d, float a)
	: parentAngle(p), maxDistance(d), angle(a){ ; }
	float parentAngle;
	float maxDistance;
	float angle;
};

class ParentDistanceCloudAnalysis : public Analysis
{
public:
	ParentDistanceCloudAnalysis();
	~ParentDistanceCloudAnalysis();
	virtual bool serialise(picojson::value& v) const;
	virtual bool deserialise(const picojson::value& v);
	virtual void updateImpl();
private:
	std::vector<ParentDistanceAngle> data;
};

