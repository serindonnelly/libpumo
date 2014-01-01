#pragma once
#include "Analysis.h"
#include "Forest.h"
class ForestAnalysis :
	public Analysis
{
public:
	ForestAnalysis();
	~ForestAnalysis();
	const Forest* getForest() { return f; };
protected:
	Forest* f;
private:
	virtual bool serialise(picojson::value& v) const;
	virtual bool deserialise(const picojson::value& v);
};

