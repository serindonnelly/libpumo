#include "ForestAnalysis.h"
#include "common.h"

ForestAnalysis::ForestAnalysis()
{
}


ForestAnalysis::~ForestAnalysis()
{
}


/***********************************************************************
 *  Method: ForestAnalysis::serialise
 *  Params: picojson::value &v
 * Returns: void
 * Effects: 
 ***********************************************************************/
bool
ConcreteForestAnalysis::serialise(picojson::value &v) const
{
	picojson::object vo;
	vo["swc"] = picojson::value(f->getSWCString());
	v = picojson::value(vo);
	return true;
}


/***********************************************************************
 *  Method: ForestAnalysis::deserialise
 *  Params: const picojson::value &v
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
ConcreteForestAnalysis::deserialise(const picojson::value &v)
{
	std::string swc;
	if (!jat(swc,v,"swc")) return false;
	std::stringstream ss(swc);
	f = new Forest();
	f->readSWC(ss, false);
	return true; // does not guarantee valid tree
}

