#pragma once
#include <vector>
#include "Forest.h"
#include "Analysis.h"
class MCExampleAnalysis :
	public Analysis
{
public:
	MCExampleAnalysis();
	~MCExampleAnalysis();
	Forest* original;
	WidthGroup originalWidth;
	std::vector<Forest*> examples;
	std::vector<float> alteredFractions;
	std::vector<WidthGroup> widths;
private:
	virtual void updateImpl();
	virtual bool serialise(picojson::value& v) const;
	virtual bool deserialise(const picojson::value& v);
	int mTreeCount = 5;
};

