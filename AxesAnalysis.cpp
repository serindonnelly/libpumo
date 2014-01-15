#include <Eigen/Core>
#include <Eigen/Eigenvalues>
#include <Eigen/Geometry>
#include "AxesAnalysis.h"
#include "ForestAnalysis.h"
#include "Forest.h"
#include "common.h"


AxesAnalysis::AxesAnalysis() : pfAxis(0, 0, 0), longAxis(0, 0, 0), sideAxis(0, 0, 0)
{
}


AxesAnalysis::~AxesAnalysis()
{
}

template<typename T>
inline T sum(const std::vector<T>& v, T start)
{
	for (const auto& t : v){ start += t; } return start;
}

template<typename T>
inline T mean(const std::vector<T>& v, T zero)
{
	return sum(v,zero) / v.size();
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
	f->samplePoints(samples, 5000000);

	// parallel fibre axis is estimated by axis that minimises variance
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

	// long axis is estimated by axis that passes through the mean point
	const Point soma = f->getNode(1)->getPosition();
	std::vector<vec> samplesProj;
	for (const auto& p : samples)
	{
		samplesProj.push_back(pfAxis.projectOrth(p));
	}
	vec meanProj = mean(samplesProj, vec(0.f, 0.f, 0.f));
	vec longAxisSuggestion = meanProj - pfAxis.projectOrth(soma);
	longAxis = vecN(pfAxis.projectOrth(longAxisSuggestion));

	Eigen::Vector3f ln(longAxis.x, longAxis.y, longAxis.z);
	Eigen::Vector3f side = pf.cross(ln);
	vec sideSuggestion = vec(side.x(), side.y(), side.z());
	sideAxis = vecN(longAxis.projectOrth(pfAxis.projectOrth(sideSuggestion)));
}


/***********************************************************************
 *  Method: AxesAnalysis::serialise
 *  Params: picojson::value &v
 * Returns: void
 * Effects: 
 ***********************************************************************/
bool
AxesAnalysis::serialise(picojson::value &v) const
{
	picojson::object pfo;
	picojson::object lno;
	picojson::object sdo;

	pfo["x"] = picojson::value(pfAxis.x);
	pfo["y"] = picojson::value(pfAxis.y);
	pfo["z"] = picojson::value(pfAxis.z);

	lno["x"] = picojson::value(longAxis.x);
	lno["y"] = picojson::value(longAxis.y);
	lno["z"] = picojson::value(longAxis.z);

	sdo["x"] = picojson::value(sideAxis.x);
	sdo["y"] = picojson::value(sideAxis.y);
	sdo["z"] = picojson::value(sideAxis.z);

	picojson::object vo;
	vo["parallel fibre axis"] = picojson::value(pfo);
	vo["long axis"] = picojson::value(lno);
	vo["side axis"] = picojson::value(sdo);

	v = picojson::value(vo);
	return true;
}


/***********************************************************************
 *  Method: AxesAnalysis::deserialise
 *  Params: const picojson::value &v
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
AxesAnalysis::deserialise(const picojson::value &v)
{
	vec pfProvisional(0.f, 0.f, 0.f);
	vec lnProvisional(0.f, 0.f, 0.f);
	vec sdProvisional(0.f, 0.f, 0.f);

	if (!jat(pfProvisional, v, "parallel fibre axis")) return false;
	if (!jat(lnProvisional, v, "long axis")) return false;
	if (!jat(sdProvisional, v, "side axis")) return false;

	if (pfProvisional.norm2() - 1.f > 0.0001) return false;
	if (lnProvisional.norm2() - 1.f > 0.0001) return false;
	if (sdProvisional.norm2() - 1.f > 0.0001) return false;

	if (pfProvisional.dot(lnProvisional) > 0.0001) return false;
	if (lnProvisional.dot(sdProvisional) > 0.0001) return false;
	if (sdProvisional.dot(pfProvisional) > 0.0001) return false;

	pfAxis = vecN(pfProvisional);
	longAxis = vecN(lnProvisional);
	sideAxis = vecN(sdProvisional);

	return true;
}


