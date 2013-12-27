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
void
ForestAnalysis::load()
{
	if (f)
		delete f;
	f = new Forest(getFilename());
}


/***********************************************************************
 *  Method: ForestAnalysis::save
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
ForestAnalysis::save()
{
	f->write(getFilename());
}


