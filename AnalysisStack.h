#pragma once
#include <map>
#include <string>
#include <list>
//#include <rapidjson/document.h>
#include <picojson.h>
#include "Analysis.h"
#include "ListAnalysis.h"
class AnalysisStack
{
public:
	AnalysisStack(std::string filename);
	~AnalysisStack();
private:
	std::map<std::string, Analysis*> mStack;
	std::vector<std::string> mTopologicalOrder;
	bool addAnalysis(const picojson::value& a);
	bool registerAnalysis(Analysis* a, const std::string& name);
	bool addList(const std::string& to, const picojson::value& ids, const picojson::value& from);
	bool addFile(const std::string& to, const picojson::value& ids, const picojson::value& from);
	bool addProcessing(const std::string& routine, const std::string& to, const picojson::value& ids, const picojson::value& from);
	bool expandList(const picojson::value& input, std::list<std::string>& output);
	bool expandList(ListAnalysis& input, std::list<std::string>& output);
	std::string readDirectory;
	std::string writeDirectory;
};

