#include <iostream>
#include <fstream>
#include <set>
#include <list>
//#include <rapidjson/filereadstream.h>
#include "AnalysisStack.h"
#include "LogStream.h"
#include "ListAnalysis.h"
#include "SWCAnalysis.h"
#include "ForestLoadAnalysis.h"
#include "ForestReduceAnalysis.h"
#include "AxesAnalysis.h"
#include "AngleDistributionAnalysis.h"
#include "MonteCarloAnalysis.h"
#include "MCExampleAnalysis.h"
#include "HeightDistributionAnalysis.h"
#include "ParentDistributionAnalysis.h"
#include "DistanceDistributionAnalysis.h"
#include "MaxDistanceDistributionAnalysis.h"
#include "ProjectionAnalysis.h"
#include "common.h"

AnalysisStack::~AnalysisStack()
{
}

/***********************************************************************
 *  Method: AnalysisStack::AnalysisStack
 *  Params: std::string filename
 * Effects: 
 ***********************************************************************/
AnalysisStack::AnalysisStack(std::string filename)
{
	std::ifstream file(filename);
	picojson::value document;
	picojson::parse(document, file);
	assert(document.is<picojson::object>());
	picojson::object& doco = document.get<picojson::object>();

	readDirectory = doco.at("read_directory").get<std::string>();
	writeDirectory = doco.at("write_directory").get<std::string>();
	picojson::array& stackDefinition = doco.at("analysis_stack").get<picojson::array>();
	//assert(document.is<picojson::array>());
	for (const auto& it : stackDefinition)
	{
		addAnalysis(it);
	}
	picojson::array firstp;
	std::list<std::string> first;
	if (jat(firstp, document, "first"))
		expandList(picojson::value(firstp), first);

	picojson::array lastp;
	std::list<std::string> last;
	if (jat(lastp, document, "last"))
	for (const auto& a : lastp)
		expandList(picojson::value(lastp), last);

	sort(first, last);
	//for (const auto& it : mTopologicalOrder)
	//{
	//	std::cout << it << " ";
	//}
	//std::cout << "\n";
	for (const auto& it : mTopologicalOrder)
	{
		mStack[it]->update();
	}
}


/***********************************************************************
 *  Method: AnalysisStack::addAnalysis
 *  Params: const picojson::value& a
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
AnalysisStack::addAnalysis(const picojson::value& a)
{
	assert(a.is<picojson::object>());
	const picojson::object& ao(a.get<picojson::object>());
	const std::string& routine = ao.at("routine").get<std::string>();
	if (routine.length() == 0 || routine[0] == '#')
		return false;
	const std::string& to = ao.at("to").get<std::string>();

	const picojson::value& ids = ao.at("ids");
	assert(ids.is<std::string>() || ids.is<picojson::array>());

	const picojson::value& from = ao.at("from");
	assert(from.is<std::string>() || from.is<picojson::array>());

	if (routine == "makelist")
	{
		addList(to, ids, from);
	}
	else if (routine == "specifyfile")
	{
		addFile(to, ids, from);
	}
	else
	{
		addProcessing(routine, to, ids, from);
	}
	return true;
}


/***********************************************************************
 *  Method: AnalysisStack::addList
 *  Params: std::string &to, const picojson::value& ids, const picojson::value& from
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
AnalysisStack::addList(const std::string &to, const picojson::value& ids, const picojson::value& from)
{
	ListAnalysis* L = new ListAnalysis();
	//L->setIdentity(to);
	std::list<std::string> idsFull;
	std::list<std::string> fromFull;
	expandList(ids, idsFull);
	expandList(from, fromFull);
	for (auto ii = idsFull.begin(); ii != idsFull.end(); ++ii)
	{
		for (auto fi = fromFull.begin(); fi != fromFull.end(); ++fi)
		{
			L->contents.push_back(*fi + *ii);
		}
	}
	//mStack[to] = L;
	registerAnalysis(L, to);
	return true;
}


/***********************************************************************
 *  Method: AnalysisStack::addFile
 *  Params: std::string &to, const picojson::value& ids, const picojson::value& from
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
AnalysisStack::addFile(const std::string &to, const picojson::value& ids, const picojson::value& from)
{
	std::list<std::string> idsFull;
	expandList(ids, idsFull);
	std::list<std::string> fromFull;
	expandList(from, fromFull);
	assert(idsFull.size() == fromFull.size());
	auto ii = idsFull.begin();
	auto fi = fromFull.begin();
	for (;
		ii != idsFull.end() && fi != fromFull.end();
		++ii, ++fi)
	{
		SWCAnalysis* f = new SWCAnalysis(readDirectory + *fi);
		registerAnalysis(f, to + *ii);
		//f->setIdentity(to + *ii);
		//mStack[to + *ii] = f;
	}
	return true;
}


/***********************************************************************
 *  Method: AnalysisStack::addProcessing
 *  Params: std::string routine, std::string &to, const picojson::value& ids, const picojson::value& from
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
AnalysisStack::addProcessing(const std::string& routine, const std::string &to, const picojson::value& ids, const picojson::value& from)
{
	std::list<std::string> idsFull;
	expandList(ids, idsFull);
	std::list<std::string> fromFull;
	expandList(from, fromFull);
	for (auto ii : idsFull)
	{
		Analysis* a = nullptr;
		bool dontRegister = false;
		if (routine == "loadswc")
		{
			a = new ForestLoadAnalysis();
		}
		else if (routine == "reduce")
		{
			a = new ForestReduceAnalysis();
		}
		else if (routine == "axes")
		{
			a = new AxesAnalysis();
		}
		else if (routine == "angledistribution")
		{
			a = new AngleDistributionAnalysis();
		}
		else if (routine == "montecarlo")
		{
			a = new MonteCarloAnalysis();
		}
		else if (routine == "mcexamples")
		{
			a = new MCExampleAnalysis();
		}
		else if (routine == "lpproject")
		{
			a = new LPProjectionAnalysis();
		}
		else if (routine == "lsproject")
		{
			a = new LSProjectionAnalysis();
		}
		else if (routine == "spproject")
		{
			a = new SPProjectionAnalysis();
		}
		else if (routine == "parentdistribution")
		{
			a = new ParentDistributionAnalysis();
		}
		else if (routine == "heightdistribution")
		{
			a = new HeightDistributionAnalysis();
		}
		else if (routine == "distancedistribution")
		{
			a = new DistanceDistributionAnalysis();
		}
		else if (routine == "maxdistancedistribution")
		{
			a = new MaxDistanceDistributionAnalysis();
		}


		if (a)
		{
			for (auto fi : fromFull)
			{
				a->addInput(mStack.at(fi + ii));
			}
			registerAnalysis(a, to + ii);
		}
	}
	
	return true;
}


/***********************************************************************
 *  Method: AnalysisStack::expandList
 *  Params: const picojson::value& input, std::list<std::string> &output
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
AnalysisStack::expandList(const picojson::value& input, std::list<std::string> &output)
{
	if (input.is<picojson::array>())
	{
		for (auto& it : input.get<picojson::array>())
		{
			expandList(it, output);
		}
	}
	else if (input.is<std::string>())
	{
		std::string s = input.get<std::string>();
		if (s.length() >0 && s[0] == '@')
		{
			ListAnalysis* L = (ListAnalysis*)mStack[s.substr(1, s.length() - 1)];
			expandList(*L, output);
		}
		else
		{
			output.push_back(s);
		}
	}
	return true;
}


/***********************************************************************
 *  Method: AnalysisStack::expandList
 *  Params: ListAnalysis &input, std::list<std::string> &output
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
AnalysisStack::expandList(ListAnalysis &input, std::list<std::string> &output)
{
	for (auto ii : input.contents)
		output.push_back(ii);
	return true;
}


/***********************************************************************
 *  Method: AnalysisStack::registerAnalysis
 *  Params: Analysis *a, const std::string &name
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
AnalysisStack::registerAnalysis(Analysis *a, const std::string &name)
{
	mStack[name] = a;
	mTopologicalOrder.push_back(name);
	a->setIdentity(name, mTopologicalOrder.size() - 1);
	a->setFilename(writeDirectory + name + ".json");
	
	//std::cout << "\n" << currentTimeString() <<  " Registered analysis " << name << std::endl;
	// a->update();
	// TODO attempt to load before updating
	// TODO make this line conditional on date of loaded analysis
	// TODO split out this line
	// TODO make this line conditional on analysis request
	// TODO make multiple requests possible
	return true;
}


/***********************************************************************
 *  Method: AnalysisStack::sort
 *  Params: std::vector<std::string> first, std::vector<std::string> last
 * Returns: void
 * Effects: alter mTopologicalOrder such that the set of its contents is unchanged,
            and it is sorted according to the partial order defined by analysis precedence,
			and the contents of first are as early as possible,
			and the contents of late are as late as possible.
			Other changes to order are minimised.
 ***********************************************************************/
void
AnalysisStack::sort(std::list<std::string>& first, std::list<std::string>& last)
{
	int size = mTopologicalOrder.size();
	std::remove_if(first.begin(), first.end(),
		[&](const std::string& s)->bool
	{return mStack.find(s) == mStack.end(); }
	);
	std::remove_if(last.begin(), last.end(),
		[&](const std::string& s)->bool
	{return mStack.find(s) == mStack.end(); }
	);
	std::set<std::string> firstS(first.begin(), first.end());
	std::set<std::string> lastS(last.begin(), last.end());
	std::list<std::string> middle;
	for (auto& s : mTopologicalOrder)
	{
		if (firstS.find(s) == firstS.end() && lastS.find(s) == lastS.end())
			middle.push_back(s);
	}
	mTopologicalOrder.clear();
	std::set<std::string> present;
	for (const auto& it : first) dependencyWalk(it,present);
	for (const auto& it : middle) dependencyWalk(it,present);
	for (const auto& it : last) dependencyWalk(it,present);
	assert(mTopologicalOrder.size() == size);
}


/***********************************************************************
 *  Method: AnalysisStack::dependencyWalk
 *  Params: const std::string &insert, std::set<std::string> &present
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
AnalysisStack::dependencyWalk(const std::string &insert, std::set<std::string> &present)
{
	if (present.find(insert) != present.end()) return;
	const Analysis* a = mStack.at(insert);
	for (const auto& input : a->getInputs())
	{
		dependencyWalk(input->getIdentity(), present);
	}
	mTopologicalOrder.push_back(insert);
	present.insert(insert);
}


