#include "ForestReduceAnalysis.h"



ForestReduceAnalysis::ForestReduceAnalysis()
{
}


ForestReduceAnalysis::~ForestReduceAnalysis()
{
}
/***********************************************************************
 *  Method: ForestReduceAnalysis::updateImpl
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
ForestReduceAnalysis::updateImpl()
{
	if (f)
		delete f;
	ForestAnalysis* fi = (ForestAnalysis*)inputs[0];
	f = new Forest(*fi->getForest());
	f->reduce();
}


