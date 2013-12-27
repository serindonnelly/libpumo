#pragma once
#include "Analysis.h"
#include "Forest.h"
class ForestAnalysis :
	public Analysis
{
public:
	ForestAnalysis();
	~ForestAnalysis();
	virtual void load();
	const Forest* getForest() { return f; };
protected:
	Forest* f;
private:
	virtual void save();
};

