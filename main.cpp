#include <stdio.h>
#include <iostream>
#include <Eigen/Dense>
#include "AnalysisStack.h"
#include "vec.h"
#include "Forest.h"
#include "common.h"


int main(int argc, char** argv)
{
	//AnalysisStack* analysisStack = new AnalysisStack(argv[1]);
	AnalysisStack* analysisStack = new AnalysisStack("analysis_stack.json");
	//Forest* f = new Forest("E:\\Dropbox\\phdcode\\swc-all\\scaled\\cell11_x40_scaled.swc");
	//if (!f->validate())
	//	return 5;
	//Forest* g = new Forest(*f);
	//if (!g->validate())
	//	return 10;
	//g->reduce();
	//if (!g->validate())
	//	return 15;
	//Forest* h = f->generateForest([](Node* n, float& angle) -> bool {angle = 0.f; return true; }, vecN(0.f, 0.f, 1.f));
	//if (!h->validate())
	//	return 20;
	//h->write("E:\\Dropbox\\phdcode\\swc-all\\scaled\\cell11_x40_scaled_flat_test.swc");
	

	return 0;
}