#include "SWCAnalysis.h"


SWCAnalysis::SWCAnalysis(std::string& fn) : SWCfilename(fn)
{
}


SWCAnalysis::~SWCAnalysis()
{
}
/***********************************************************************
 *  Method: SWCAnalysis::serialise
 *  Params: picojson::value &v
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
SWCAnalysis::serialise(picojson::value &v) const
{
	return false;
}


/***********************************************************************
 *  Method: SWCAnalysis::deserialise
 *  Params: const picojson::value &v
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
SWCAnalysis::deserialise(const picojson::value &v)
{
	return false;
}


