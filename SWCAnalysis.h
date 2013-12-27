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
	virtual void load(){};
private:
	std::string SWCfilename;
	virtual void save(){};
	virtual void updateImpl(){};
};

