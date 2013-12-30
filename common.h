#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <random>
#include <cmath>
#include <picojson.h>

static std::random_device rd;
static std::mt19937 gen(rd());

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

const float PI = 3.14159265358979323846f;
static const float RAD_TO_DEG = 180.f/PI;