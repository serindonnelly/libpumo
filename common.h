#include <string>
#include <fstream>
#include <iostream>
#include <picojson.h>

//#include "AnalysisStack.h"
//static AnalysisStack* analysisStack;

picojson::value loadJSON(const std::string& filename)
{
	std::ifstream f(filename);
	picojson::value v;
	picojson::parse(v, f);
	return v;
}

void saveJSON(const std::string& filename)