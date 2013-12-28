#include "Analysis.h"
#include "common.h"


Analysis::Analysis()
{
	mUpdated = (time_t)0;
}


Analysis::~Analysis()
{
}


/***********************************************************************
 *  Method: Analysis::addInput
 *  Params: std::string &input
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Analysis::addInput(Analysis* input)
{
	inputs.push_back(input);
	allInputs.push_back(input);
	for (auto ii : input->allInputs)
	{
		allInputs.push_back(ii);
	}
}


/***********************************************************************
 *  Method: Analysis::getUpdateTime
 *  Params: 
 * Returns: time_t
 * Effects: 
 ***********************************************************************/
time_t
Analysis::getUpdateTime()
{
	return mUpdated;
}


/***********************************************************************
 *  Method: Analysis::update
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Analysis::update()
{
	updateImpl();
	mUpdated = time(nullptr);
	//save();
}


/***********************************************************************
 *  Method: Analysis::setIdentity
 *  Params: std::string id
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Analysis::setIdentity(std::string id, int order)
{
	mIdentity = id;
	mOrder = order;
}

