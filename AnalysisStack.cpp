#include <iostream>
#include <fstream>
#include <list>
//#include <rapidjson/filereadstream.h>
#include "AnalysisStack.h"
#include "ListAnalysis.h"
#include "SWCAnalysis.h"
#include "ForestLoadAnalysis.h"
#include "ForestReduceAnalysis.h"


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
	assert(document.is<picojson::array>());
	for (const auto& it : document.get<picojson::array>())
	{
		addAnalysis(it);
	}

	//rapidjson::Document document;
	////std::ifstream file(filename);
	//FILE* f = fopen(filename.c_str(), "r");
	//rapidjson::FileStream is(f);
	//rapidjson::FileReadStream iss;
	//document.ParseStream<0>(is);
	//assert(document.IsArray());
	//for (rapidjson::SizeType i = 0; i != document.Size(); i++)
	//{
	//	rapidjson::Value a;
	//	a = document[i];
	//	addAnalysis(a);
	//}
}


/***********************************************************************
 *  Method: AnalysisStack::addAnalysis
 *  Params: rapidjson::Value a
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
AnalysisStack::addAnalysis(const picojson::value& a)
{
	assert(a.is<picojson::object>());
	const picojson::object& ao(a.get<picojson::object>());
	const std::string& routine = ao.at("routine").get<std::string>();

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
	L->setIdentity(to);
	std::list<std::string> idsFull;
	std::list<std::string> fromFull;
	expandList(ids, idsFull);
	expandList(from, fromFull);
	for (auto ii = idsFull.begin(); ii != idsFull.end(); ++ii)
	{
		for (auto fi = fromFull.begin(); fi != fromFull.end(); ++fi)
		{
			L->contents.push_back(*ii + *fi);
		}
	}
	mStack[to] = L;
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
		SWCAnalysis* f = new SWCAnalysis(*fi);
		f->setIdentity(to + *ii);
		mStack[to + *ii] = f;
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
		Analysis* a;
		a = new ForestLoadAnalysis(); // kill this line with fire
		if (routine == "parseswc")
		{
			a = new ForestLoadAnalysis();
		}
		else if (routine == "reduce")
		{
			a = new ForestReduceAnalysis();
		}
		//else if (routine == "axes")
		//{
		//	
		//}
		//else if (routine == "angledistribution")
		//{

		//}
		//else if (routine == "anglemc")
		//{

		//}
		//else if (routine == "parentdistribution")
		//{

		//}
		//else if (routine == "parentmc")
		//{

		//}
		//else if (routine == "heightdistribution")
		//{

		//}
		//else if (routine == "distancedistribution")
		//{

		//}
		//else if (routine == "distancemc")
		//{

		//}
		for (auto fi : fromFull)
		{
			a->addInput(mStack.at(fi + ii));
		}
		a->setIdentity(to + ii);
		mStack[to + ii] = a;
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
		if (s[0] == '@')
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


