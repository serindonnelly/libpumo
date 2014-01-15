#include <stdio.h>
#include <iostream>
#include <Eigen/Dense>
#include "AnalysisStack.h"
#include "vec.h"
#include "Forest.h"
#include "teestream.h"
#include "LogStream.h"
#include "common.h"
#include "TCLForest.h"

int main(int argc, char** argv)
{
	std::ofstream log("libpumo.log");
	teestream tlog(std::cout, log);
	logging::lerr  = new CLogger("ERROR  ", tlog);
	logging::lwarn = new CLogger("WARNING", tlog);
	logging::linfo = new CLogger("INFO   ", tlog);
	std::cout << currentTimeString() << " Beginning log" << std::endl;
	//*logging::linfo << "Testing" << std::endl;
	//AnalysisStack* analysisStack = new AnalysisStack(argv[1]);
	AnalysisStack* analysisStack = new AnalysisStack("analysis_stack_all.json");
	return 0;
}