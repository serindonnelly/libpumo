#pragma once
#include "Analysis.h"
#include "WidthGroup.h"
class MonteCarloAnalysis :
	public Analysis
{
public:
	MonteCarloAnalysis();
	~MonteCarloAnalysis();
	std::vector<WidthGroup> widths;
	std::vector<float> alteredFractions;
	WidthGroup mean;
	WidthGroup SD;
	WidthGroup originalWidth;
	WidthGroup discrepancy;
private:
	virtual void updateImpl();
	virtual bool serialise(picojson::value& v) const;
	virtual bool deserialise(const picojson::value& v);
	int mTreeCount = 500;
};

