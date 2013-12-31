#include <stdio.h>
#include <iostream>
#include <Eigen/Dense>
#include "AnalysisStack.h"
#include "vec.h"
#include "Forest.h"
#include "common.h"
#include "teestream.h"
#include "LogStream.h"

int main(int argc, char** argv)
{
	std::ofstream log("libpumo.log");
	teestream tlog(std::cout, log);
	err  = new CLogger("ERROR  ", tlog);
	warn = new CLogger("WARNING", tlog);
	info = new CLogger("INFO   ", tlog);
	*info << "Beginning log" << std::endl;
	//AnalysisStack* analysisStack = new AnalysisStack(argv[1]);
	AnalysisStack* analysisStack = new AnalysisStack("analysis_stack.json");
	return 0;
}