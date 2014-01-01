#pragma once
#include "Analysis.h"
class MonteCarloAnalysis :
	public Analysis
{
public:
	MonteCarloAnalysis();
	~MonteCarloAnalysis();
	std::vector<float> widths;
private:
	virtual void updateImpl();
	virtual bool serialise(picojson::value& v) const;
	virtual bool deserialise(const picojson::value& v);
	int mTreeCount = 3;
};

