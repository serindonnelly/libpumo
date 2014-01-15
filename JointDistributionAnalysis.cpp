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
		return false; // distribution doesn't want to alter the node
	if (mConditionalDistributions[chosenDistribution] == nullptr)
		return false; // distribution has no samples for this condition
	newAngle = (*mConditionalDistributions[chosenDistribution])(gen)/RAD_TO_DEG;
	return true;
}




void JointDistributionAnalysis::updateImpl()
{
	std::vector<float> samplesX;
	std::vector<float> samplesY;

	mHistogram.setBinCountX(preferredBinCountX());
	mHistogram.setBinCountY(preferredBinCountY());
	mHistogram.zero();
	std::vector<float> sampleWeights;

	collectSample(samplesX, samplesY, sampleWeights);
	float minX, maxX;
	if (!preferredBinBoundariesX(minX, maxX))
	{
		mHistogram.guessRangeX(samplesX);
	}
	else
	{
		mHistogram.setRangeX(minX, maxX);
	}

	float minY, maxY;
	if (!preferredBinBoundariesY(minY, maxY))
	{
		mHistogram.guessRangeY(samplesY);
	}
	else
	{
		mHistogram.setRangeY(minY, maxY);
	}

	int count = std::min({ samplesX.size(), samplesY.size(), sampleWeights.size() });
	for (int i = 0; i < count; i++)
	{
		mHistogram.insertSample(samplesX[i], samplesY[i], sampleWeights[i]);
	}
	updateConditionalDistributions();
	
}
/***********************************************************************
 *  Method: JointDistributionAnalysis::serialise
 *  Params: picojson::value &v
 * Returns: void
 * Effects: 
 ***********************************************************************/
bool
JointDistributionAnalysis::serialise(picojson::value &v) const
{
	picojson::value vadd;
	if (!serialiseAdditional(vadd)) return false;
	picojson::object vo;
	vo["additional"] = vadd;

	vo["bin_count_x"] = picojson::value((double)mHistogram.getBinCountX());
	vo["bin_count_y"] = picojson::value((double)mHistogram.getBinCountY());

	vo["min_x"] = picojson::value((double)mHistogram.getMinX());
	vo["max_x"] = picojson::value((double)mHistogram.getMaxX());
	vo["min_y"] = picojson::value((double)mHistogram.getMinY());
	vo["max_y"] = picojson::value((double)mHistogram.getMaxY());


	picojson::array va;
	for (int i = 0; i < mHistogram.getBinCountX(); i++)
	{
		for (int j = 0; j < mHistogram.getBinCountY(); j++)
		{
			va.push_back(picojson::value((double)mHistogram(i, j)));
		}
	}
	vo["weights"] = picojson::value(va);
	v = picojson::value(vo);
	return true;
}


/***********************************************************************
 *  Method: JointDistributionAnalysis::deserialise
 *  Params: const picojson::value &v
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
JointDistributionAnalysis::deserialise(const picojson::value &v)
{
	float minX, maxX, minY, maxY;
	if (!jat(minX, v, "min_x")) return false;
	if (!jat(maxX, v, "max_x")) return false;
	if (!jat(minY, v, "min_y")) return false;
	if (!jat(maxY, v, "max_y")) return false;

	int binCountX, binCountY;
	if (!jat(binCountX, v, "bin_count_x")) return false;
	if (!jat(binCountY, v, "bin_count_y")) return false;

	picojson::array weights;
	if (!jat(weights, v, "weights")) return false;
	if (weights.size() != binCountX * binCountY) return false;
	for (const auto& weight : weights)
	{
		float w;
		if (!jget(w, weight)) return false;
		if (w < 0.f) return false;
	}

	picojson::value vadd;
	if (v.contains("additional"))
		vadd = v.get("additional");
	if (!deserialiseAdditional(vadd)) return false;

	mHistogram.setRangeX(minX, maxX);
	mHistogram.setRangeY(minY, maxY);
	mHistogram.setBinCountX(binCountX);
	mHistogram.setBinCountY(binCountY);
	mHistogram.zero();

	int c = 0;
	for (int i = 0; i < binCountX; i++)
	{
		for (int j = 0; j < binCountY; j++)
		{
			float w;
			jget(w, weights[c]);
			mHistogram(i, j) = w;
			//mHistogram.setEntry(i, j, w);
			c++;
		}

	}
	updateConditionalDistributions();

	return true;
}


/***********************************************************************
 *  Method: JointDistributionAnalysis::updateConditionalDistributions
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
JointDistributionAnalysis::updateConditionalDistributions()
{
	calculatePartialSums();
	for (auto* dist : mConditionalDistributions)
	{
		delete dist;
	}
	mConditionalDistributions.clear();
	for (int i = 0; i < mHistogram.getBinCountY(); i++)
	{
		std::vector<float> binWeights;
		for (int j = 0; j < mHistogram.getBinCountX(); j++)
		{
			binWeights.push_back(mHistogram(j, i));
		}
		std::vector<float> binBoundariesX;
		mHistogram.getBinBoundariesX(binBoundariesX);
		std::piecewise_constant_distribution<float>* dist = nullptr;
		for (float w : binWeights)
		{
			if (w > 0.f)
			{
				dist = new std::piecewise_constant_distribution<float>(
					binBoundariesX.begin(),
					binBoundariesX.end(),
					binWeights.begin());
			}
		}
		mConditionalDistributions.push_back(dist);
	}
}


/***********************************************************************
*  Method: JointDistributionAnalysis::calculatePartialSums
*  Params:
* Returns: void
* Effects:
***********************************************************************/
void
JointDistributionAnalysis::calculatePartialSums()
{
	// sum rows
	binTotals.clear();
	for (int y = 0; y < mHistogram.getBinCountY(); y++)
	{
		binTotals.push_back(0.f);
		for (int x = 0; x < mHistogram.getBinCountX(); x++)
		{
			binTotals[y] += mHistogram(x, y);
		}
	}
}
