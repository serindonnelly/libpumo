#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>
#include <cmath>
#include <picojson.h>
#include <ctime>
#include <iomanip>
#include "WidthGroup.h"
#include "vec.h"

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
static bool jget(WidthGroup& out, const picojson::value& v)
{
	WidthGroup wg(v);
	if (!wg.isValid())
		return false;
	out = wg;
	return true;
}

static bool jget(vec& out, const picojson::value& v)
{
	if (!v.is<picojson::object>()) return false;
	if (!v.contains("x")) return false;
	if (!v.contains("y")) return false;
	if (!v.contains("z")) return false;
	picojson::value x, y, z;
	x = v.get("x");
	y = v.get("y");
	z = v.get("z");
	if (!x.is<double>()) return false;
	if (!y.is<double>()) return false;
	if (!z.is<double>()) return false;
	out.x = (float)x.get<double>();
	out.y = (float)y.get<double>();
	out.z = (float)z.get<double>();
	return true;
}

template<typename T>
static bool jat(T& out, const picojson::value& v, const std::string& key)
{
	if (!v.is<picojson::object>())
		return false;
	if (!v.contains(key))
		return false;
	return jget(out, v.get(key));
}

static std::string currentTimeString(std::string fmt = "%H:%M:%S")
{
	auto t = std::time(nullptr);
	//std::localtime not thread safe
#pragma warning(suppress: 4996)
	auto tm = *std::localtime(&t);
	//#pragma warning(pop)
	std::stringstream ss;
	ss << std::put_time(&tm, fmt.c_str());
	return ss.str();
}

const float PI = 3.14159265358979323846f;
static const float RAD_TO_DEG = 180.f/PI;