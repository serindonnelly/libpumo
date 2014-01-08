#pragma once
#include <string>
#include "NullAnalysis.h"

class SWCAnalysis :
	public NullAnalysis
{
public:
	SWCAnalysis(std::string& fn);
	~SWCAnalysis();
	std::string getSWCFilename() { return SWCfilename; };
private:
	std::string SWCfilename;

};

