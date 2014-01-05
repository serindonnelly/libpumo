#include <iostream>
#include "ForestLoadAnalysis.h"
#include "SWCAnalysis.h"
#include "common.h"

/***********************************************************************
 *  Method: ForestLoadAnalysis::ForestLoadAnalysis
 *  Params: 
 * Effects: 
 ***********************************************************************/
ForestLoadAnalysis::ForestLoadAnalysis()
{
}


/***********************************************************************
 *  Method: ForestLoadAnalysis::~ForestLoadAnalysis
 *  Params: 
 * Effects: 
 ***********************************************************************/
ForestLoadAnalysis::~ForestLoadAnalysis()
{
}


/***********************************************************************
 *  Method: ForestLoadAnalysis::updateImpl
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
ForestLoadAnalysis::updateImpl()
{
	if (f)
		delete f;
	std::string fn = ((SWCAnalysis*)inputs[0])->getSWCFilename();
	std::cout << currentTimeString() << " Reading forest " << fn << "\n";
	f = new Forest(fn,false);
}


