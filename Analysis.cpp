
#include <sys/stat.h>
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
	//allInputs.push_back(input);
	//for (auto ii : input->allInputs)
	//{
	//	allInputs.push_back(ii);
	//}
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
		updateImpl();
		mUpdated = time(nullptr);
		std::cout << currentTimeString() << " Updated analysis " << mIdentity << std::endl;
		mUpdated = std::time(nullptr);
		save();
		std::cout << std::endl;
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

	struct stat st;
	int sst = stat(mFilename.c_str(), &st);
	if (sst != 0)
	{
		std::cout << currentTimeString() << " File not found: " << mFilename << std::endl;
		return false;
	}
	
	for (const auto& it : inputs)
	{
		if (it->getUpdateTime() >= st.st_mtime)
		{
			std::cout << currentTimeString() << " File too old: " << mFilename << std::endl;
			return false;
		}
	}
	std::ifstream in(mFilename);
	picojson::value v;
	if (!in.good())
	{
		std::cout << currentTimeString() << " File unreadable: " << mFilename << std::endl;
		return false;
	}
	picojson::parse(v, in);
	if (!deserialise(v))
	{
		std::cout << currentTimeString() << " Deserialisation failed: " << mFilename << std::endl;
		return false;
	}
	mUpdated = st.st_mtime;
	std::cout << currentTimeString() << " Load successful: " << mFilename << std::endl;
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


