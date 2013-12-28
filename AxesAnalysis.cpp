#include <Eigen/Core>
#include <Eigen/Eigenvalues>
#include "AxesAnalysis.h"
#include "ForestAnalysis.h"
#include "Forest.h"
#include "common.h"


AxesAnalysis::AxesAnalysis() : pfAxis(0,0,0)
{
}


AxesAnalysis::~AxesAnalysis()
{
}


/***********************************************************************
 *  Method: AxesAnalysis::load
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
AxesAnalysis::load()
{
	picojson::value v;
	parseFile(v, getFilename());
	assert(v.is<picojson::object>());
	picojson::value& pf = v.get<picojson::object>().at("pf axis");
	assert(pf.is<picojson::object>());
	picojson::object& pfObj = pf.get<picojson::object>();
	pfAxis = vecN((float)pfObj.at("x").get<double>(),
				  (float)pfObj.at("y").get<double>(),
				  (float)pfObj.at("z").get<double>());
}


/***********************************************************************
 *  Method: AxesAnalysis::save
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
AxesAnalysis::save()
{
	picojson::object pf;
	pf["x"] = picojson::value(pfAxis.x);
	pf["y"] = picojson::value(pfAxis.y);
	pf["z"] = picojson::value(pfAxis.z);

	picojson::object v;
	v["pf axis"] = picojson::value(pf);

	saveJSON(getFilename(), picojson::value(v));
}


/***********************************************************************
 *  Method: AxesAnalysis::updateImpl
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
AxesAnalysis::updateImpl()
{
	std::vector<Point> samples;
	const Forest* f = ((ForestAnalysis*)inputs[0])->getForest();
	f->samplePoints(samples, 500000);
	std::vector<float> samplesFlat;

	for (const auto& p : samples)
	{
		samplesFlat.push_back(p.x);
		samplesFlat.push_back(p.y);
		samplesFlat.push_back(p.z);
	}
	Eigen::Map<Eigen::MatrixXf> data(samplesFlat.data(),3,10000);
	Eigen::Vector3f v = data.col(0);
	Eigen::MatrixXf centred = data.colwise() - data.rowwise().mean();
	Eigen::MatrixXf cov = (centred * centred.transpose())/(data.cols()-1.f);
	Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> eig(cov);
	//printf("%f\n", cov.coeff(0, 0));
	//return (int)cov.coeff(2, 2);
	Eigen::Vector3f pf = eig.eigenvectors().col(0);
	pfAxis = vecN(pf.x(), pf.y(), pf.z());
}


