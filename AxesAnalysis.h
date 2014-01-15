#pragma once
#include "Analysis.h"
#include "vecN.h"
class AxesAnalysis : public Analysis
{
public:
	AxesAnalysis();
	~AxesAnalysis();
	vecN getPF() { return pfAxis; }
	vecN getLong() { return longAxis; }
	vecN getSide() { return sideAxis; }
private:
	virtual bool serialise(picojson::value& v) const;
	virtual bool deserialise(const picojson::value& v);
	virtual void updateImpl();
	vecN pfAxis;
	vecN longAxis;
	vecN sideAxis;
};

