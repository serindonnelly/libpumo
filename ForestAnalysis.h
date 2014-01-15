#pragma once
#include "Analysis.h"
#include "Forest.h"
class ForestAnalysis :
	public Analysis
{
public:
	ForestAnalysis();
	virtual ~ForestAnalysis();
	virtual const Forest* getForest() const = 0;
	virtual Forest* getForest() = 0;
private:
};

class ConcreteForestAnalysis :
	public ForestAnalysis
{
public:
	~ConcreteForestAnalysis() { if(f) delete f; }
	virtual const Forest* getForest() const { return f; }
	virtual Forest* getForest(){ return f; }
protected:
	Forest* f;
	virtual bool serialise(picojson::value& v) const;
	virtual bool deserialise(const picojson::value& v);
};

