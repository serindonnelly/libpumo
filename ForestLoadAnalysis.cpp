#include <iostream>
#include "ForestLoadAnalysis.h"
#include "SWCAnalysis.h"

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
	std::cout << "reading forest " << fn << "\n";
	f = new Forest(fn,false);
}


