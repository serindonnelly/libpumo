#pragma once
#include <vector>
#include "ForestAnalysis.h"
#include "Analysis.h"
class MCExampleAnalysis :
	public ForestAnalysis
{
public:
	MCExampleAnalysis();
	~MCExampleAnalysis();
	Forest* original;
	WidthGroup originalWidth;
	float originalLength;
	std::vector<Forest*> examples;
	std::vector<float> alteredFractions;
	std::vector<WidthGroup> widths;
	std::vector<float> lengths;
	virtual const Forest* getForest() const { return examples[0]; }
	virtual Forest* getForest() { return examples[0]; }
	virtual void initForest() { Forest* f = new Forest; examples.push_back(f); }
private:
	virtual void updateImpl();
	virtual bool serialise(picojson::value& v) const;
	virtual bool deserialise(const picojson::value& v);
	int mTreeCount = 1; // use more when more can  be used
};

