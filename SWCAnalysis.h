#pragma once
#include <string>
#include "Analysis.h"
class SWCAnalysis :
	public Analysis
{
public:
	SWCAnalysis(std::string& fn);
	~SWCAnalysis();
	std::string getSWCFilename() { return SWCfilename; };
private:
	std::string SWCfilename;
	virtual bool serialise(picojson::value& v) const;
	virtual bool deserialise(const picojson::value& v);
	virtual void updateImpl(){};
};

