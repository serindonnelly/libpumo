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
	//std::vector<Point> samples;
	//f->samplePoints(samples, 500000);
	//std::vector<float> samplesFlat;

	//for (const auto& p : samples)
	//{
	//	samplesFlat.push_back(p.x);
	//	samplesFlat.push_back(p.y);
	//	samplesFlat.push_back(p.z);
	//}
	//Eigen::Map<Eigen::MatrixXf> data(samplesFlat.data(),3,10000);
	//Eigen::Vector3f v = data.col(0);
	//Eigen::MatrixXf centred = data.colwise() - data.rowwise().mean();
	//Eigen::MatrixXf cov = (centred * centred.transpose())/(data.cols()-1.f);
	//Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> eig(cov);
	//printf("%f\n", cov.coeff(0, 0));
	////return (int)cov.coeff(2, 2);
	//Eigen::Vector3f pf = eig.eigenvectors().col(0);
	//Eigen::Vector3f pl = eig.eigenvectors().col(2);
	//Eigen::Vector3f ps = eig.eigenvectors().col(1);
	//std::cout << cov.col(0)(0) << " " << cov.col(0)(1) << " " << cov.col(0)(2) << " " << "\n";
	//std::cout << cov.col(1)(0) << " " << cov.col(1)(1) << " " << cov.col(1)(2) << " " << "\n";
	//std::cout << cov.col(2)(0) << " " << cov.col(2)(1) << " " << cov.col(2)(2) << " " << "\n";
	//std::cout << cov.col(2)(0) << " " << cov.col(2)(1) << " " << cov.col(2)(2) << " " << "\n\n";
	//std::cout << eig.eigenvectors() << "\n\n";
	//std::cout << eig.eigenvalues() << "\n\n";
	//getchar();
	//return (int)pf(2);
	//Point p = samples[0];
	//return (int)samples[500].x;
	//return g->getNode(1)->numChildren();
	return 0;
}