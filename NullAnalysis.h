#pragma once
#include "Analysis.h"
class NullAnalysis :
	public Analysis
{
public:
	NullAnalysis();
	~NullAnalysis();
private:
	virtual bool serialise(picojson::value& v) const { return false; }
	virtual bool deserialise(const picojson::value& v){ return false; }
	virtual void updateImpl(){ ; }
	virtual void save(){ ; }
	virtual void update(){ ; }
	virtual bool load(){ return false; }
};

