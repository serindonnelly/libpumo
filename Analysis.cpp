#include "Analysis.h"
#include "common.h"
#include "LogStream.h"


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
	if (!load())
	{
		std::cout << currentTimeString() << " Load failed: " << mFilename << std::endl;
		updateImpl();
		mUpdated = time(nullptr);
		std::cout << currentTimeString() << " Updated analysis " << mIdentity << std::endl;
		save();
	}
	else
	{
		std::cout << currentTimeString() << " Load successful: " << mFilename << std::endl;
	}
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

/***********************************************************************
 *  Method: Analysis::load
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
bool
Analysis::load()
{
	std::ifstream in(mFilename);
	picojson::value v;
	if (!in.good())
		return false;
	picojson::parse(v, in);
	if (!deserialise(v))
		return false;
	return true;
}


/***********************************************************************
 *  Method: Analysis::save
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Analysis::save()
{
	picojson::value v;
	if (serialise(v))
	{
		std::ofstream of(mFilename);
		if (of.good())
		{
			of << v;
			std::cout << currentTimeString() << " Saved analysis: " << mFilename << std::endl;
			return;
		}
	}	
	std::cout << currentTimeString() << " Save failed: " << mFilename << std::endl;
	// TODO encapsulate v in two-part object with date
}


