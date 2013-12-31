#include "JointDistributionAnalysis.h"
#include "common.h"


JointDistributionAnalysis::JointDistributionAnalysis()
{
}


JointDistributionAnalysis::~JointDistributionAnalysis()
{
}
/***********************************************************************
 *  Method: JointDistributionAnalysis::generateAngle
 *  Params: const Node *n
 * Returns: float
 * Effects: 
 ***********************************************************************/
bool
JointDistributionAnalysis::generateAngle(const Node *n, float& newAngle) const
{
	int chosenDistribution;
	if (!selectDistribution(n, chosenDistribution))
		return false;
	newAngle = (*mConditionalDistributions[chosenDistribution])(gen);
}


/***********************************************************************
 *  Method: JointDistributionAnalysis::zero
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
JointDistributionAnalysis::zero()
{
	// TODO
}

void JointDistributionAnalysis::updateImpl()
{
	std::vector<float> samplesX;
	std::vector<float> samplesY;
	//REPLACE THIS
	mHistogram.setBinCountX(30);
	mHistogram.setBinCountY(30);
	//BIN COUNTS MUST BE VARIABLE FOR DISTANCE DEP ANALYSIS
	mHistogram.guessRangeX(samplesX);
	mHistogram.guessRangeY(samplesY);
	// BIN RANGES MUST BE EXACTLY SETTABLE FOR ANGLE DISTRIBUTIONS ([0,180])
	std::vector<float> sampleWeights;

	collectSample(samplesX, samplesY, sampleWeights);
	int count = std::min({ samplesX.size(), samplesY.size(), sampleWeights.size() });
	for (int i = 0; i < count; i++)
	{
		mHistogram.insertSample(samplesX[i], samplesY[i], sampleWeights[i]);
	}
	// add gets for inaccessible members here
	for (int i = 0; i < mHistogram.binCountY; i++)
	{
		std::vector<float> binWeights;
		for (int j = 0; j < mHistogram.binCountX; j++)
		{
			binWeights.push_back(mHistogram(j, i));
		}
		mConditionalDistributions[i] = new std::piecewise_constant_distribution<float>(
			mHistogram.binBoundariesX.begin(),
			mHistogram.binBoundariesX.end(),
			binWeights.begin());
	}
}