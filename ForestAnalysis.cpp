#include "ForestAnalysis.h"


ForestAnalysis::ForestAnalysis()
{
}


ForestAnalysis::~ForestAnalysis()
{
}
/***********************************************************************
 *  Method: ForestAnalysis::load
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
//void
//ForestAnalysis::load()
//{
//	if (f)
//		delete f;
//	f = new Forest(getFilename());
//}


/***********************************************************************
 *  Method: ForestAnalysis::save
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
//void
//ForestAnalysis::save()
//{
//	f->write(getFilename());
//}


/***********************************************************************
 *  Method: ForestAnalysis::serialise
 *  Params: picojson::value &v
 * Returns: void
 * Effects: 
 ***********************************************************************/
bool
ForestAnalysis::serialise(picojson::value &v) const
{
	return false;
}


/***********************************************************************
 *  Method: ForestAnalysis::deserialise
 *  Params: const picojson::value &v
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
ForestAnalysis::deserialise(const picojson::value &v)
{
	return false;
}


