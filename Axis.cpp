#include <Eigen/Core>
#include <Eigen/Eigenvalues>
#include "Axis.h"
#include "ONBasis.h"


const int Axis::numSamples = 500000;

Axis::Axis(Forest* f) : mAxis(0, 0, 0), calculated(false), mForest(f)
{
}


Axis::~Axis()
{
}

/***********************************************************************
 *  Method: Axis::getAxis
 *  Params: 
 * Returns: vecN
 * Effects: 
 ***********************************************************************/
vecN
Axis::getAxis()
{
	if (!calculated)
		calculateAxis();
	return mAxis;
}




/***********************************************************************
 *  Method: PFAxis::calculateAxis
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
PFAxis::calculateAxis()
{

	std::vector<Point> samples;
	mForest->samplePoints(samples,numSamples);
	std::vector<float> samplesFlat;

	for (const auto& p : samples)
	{
		samplesFlat.push_back(p.x);
		samplesFlat.push_back(p.y);
		samplesFlat.push_back(p.z);
	}
	Eigen::Map<Eigen::MatrixXf> data(samplesFlat.data(), 3, 10000);
	Eigen::MatrixXf centred = data.colwise() - data.rowwise().mean();
	Eigen::MatrixXf cov = centred*centred.transpose();
	Eigen::SelfAdjointEigenSolver < Eigen::MatrixXf > eig(cov);
	Eigen::Vector3f ax = eig.eigenvectors().col(0);
	mAxis = vecN(ax(0), ax(1), ax(2));

	calculated = true;
}


/***********************************************************************
 *  Method: LongAxis::calculateAxis
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
LongAxis::calculateAxis()
{
	ONBasis B;
	//B.setA(mForest->mPF->getAxis());
}


/***********************************************************************
 *  Method: SideAxis::calculateAxis
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
SideAxis::calculateAxis()
{
}


