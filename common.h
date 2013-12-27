#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <picojson.h>

//#include "AnalysisStack.h"
//static AnalysisStack* analysisStack;

static void parseFile(picojson::value& out, const std::string& filename)
{
	std::ifstream f(filename);
	picojson::parse(out, f);
}

static picojson::value loadJSON(const std::string& filename)
{
	picojson::value v;
	parseFile(v, filename);
	return v; // inefficient copy; use parseFile if this is used in a bottleneck
	// check that this one actually works
}

static void saveJSON(const std::string& filename, const picojson::value& document)
{
	std::ofstream f(filename);
	f << document;
}