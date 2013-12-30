#include "MonteCarloAnalysis.h"
#include "ForestAnalysis.h"
#include "AxesAnalysis.h"
#include "DistributionAnalysis.h"


MonteCarloAnalysis::MonteCarloAnalysis()
{
}


MonteCarloAnalysis::~MonteCarloAnalysis()
{
}
/***********************************************************************
 *  Method: MonteCarloAnalysis::load
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
MonteCarloAnalysis::load()
{
}


/***********************************************************************
 *  Method: MonteCarloAnalysis::updateImpl
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
MonteCarloAnalysis::updateImpl()
{
	const Forest* f = ((ForestAnalysis*)inputs[0])->getForest();
	vecN pf = ((AxesAnalysis*)inputs[1])->getPF();
	DistributionAnalysis* dist = (DistributionAnalysis*)inputs[2];
	widths.clear();
	for (int i = 0; i < mTreeCount; i++)
	{
		Forest* ff = f->generateForest([&](const Node* n, float& newAngle) -> bool
		{
			newAngle = dist->generateAngle(n);
			return true;
		}, pf);
		widths.push_back(ff->getWidth(pf));
		delete ff;
	}
	//get mean, std of widths
}


/***********************************************************************
 *  Method: MonteCarloAnalysis::save
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
MonteCarloAnalysis::save()
{
}


