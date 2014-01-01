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

#define JGET(ctype,jtype) \
	static bool jget(ctype& out, const picojson::value& v) \
{if (!v.is<jtype>()) return false; out = (ctype)v.get<jtype>(); return true;}
JGET(int,double)
JGET(bool,bool)
JGET(float,double)
JGET(double,double)
JGET(std::string,std::string)
JGET(picojson::object, picojson::object)
JGET(picojson::array, picojson::array)
#undef JGET

template<typename T>
static bool jat(T& out, const picojson::value& v, const std::string& key)
{
	if (!v.is<picojson::object>())
		return false;
	if (!v.contains(key))
		return false;
	return jget(out, v.get(key));
}

const float PI = 3.14159265358979323846f;
static const float RAD_TO_DEG = 180.f/PI;