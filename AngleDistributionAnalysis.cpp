#include <algorithm>
#include <picojson.h>
#include "common.h"
#include "AngleDistributionAnalysis.h"
#include "ForestAnalysis.h"
#include "AxesAnalysis.h"


AngleDistributionAnalysis::AngleDistributionAnalysis()
{
}


AngleDistributionAnalysis::~AngleDistributionAnalysis()
{
}

void
AngleDistributionAnalysis::updateImpl()
{
	binCount = 180;
	zero();
	const Forest* f = ((ForestAnalysis*)inputs[0])->getForest();
	const vecN pf = ((AxesAnalysis*)inputs[1])->getPF();
	std::vector<std::pair<float,float>> angleLengthSamples;
	//std::vector<float> lengthSamples;
	//get samples
	for (const auto& n : f->getGraph())
	{
		const Node* nn = n.second;
		if (!nn->isRoot())
		{
			const Segment* s = nn->getSegment();
			float angle = RAD_TO_DEG*acos(pf.cosine(s->getVector()));
			float length = s->getVector().norm();
			angleLengthSamples.push_back(std::make_pair(angle,length));
		}
	}
	// bin samples
	for (const auto& angleLength : angleLengthSamples)
	{
		auto insert = std::lower_bound(binBoundaries.begin(), binBoundaries.end(), angleLength.first);
		//assert(*insert >= angleLength.first);
		//assert(*(insert-1) <= angleLength.first);
		int index = insert - binBoundaries.begin() - 1;
		binWeights[index] += angleLength.second;

	}
	mDistribution = std::piecewise_constant_distribution<float>(binBoundaries.begin(),binBoundaries.end(),binWeights.begin());
}
/***********************************************************************
 *  Method: AngleDistributionAnalysis::zero
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
AngleDistributionAnalysis::zero()
{
	for (int i = 0; i < binCount; i++)
	{
		binBoundaries.push_back((float)i);
		binWeights.push_back(0.f);
	}
	binBoundaries.push_back(180.f);
}



bool AngleDistributionAnalysis::generateAngle(const Node* n, float& newAngle) const
{
	newAngle = mDistribution(gen);
	return true;
}/***********************************************************************
 *  Method: AngleDistributionAnalysis::serialise
 *  Params: picojson::value &v
 * Returns: void
 * Effects: 
 ***********************************************************************/
bool
AngleDistributionAnalysis::serialise(picojson::value &v) const
{
	return false;
}


/***********************************************************************
 *  Method: AngleDistributionAnalysis::deserialise
 *  Params: const picojson::value &v
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
AngleDistributionAnalysis::deserialise(const picojson::value &v)
{
	int provisionalBinCount;
	picojson::array provisionalBinBoundaries;
	picojson::array provisionalBinWeights;

	if (!jat(provisionalBinCount, v, "bin_count")) return false;
	if (!jat(provisionalBinBoundaries, v, "bin_boundaries")) return false;
	if (!jat(provisionalBinWeights, v, "bin_weights")) return false;

	if (provisionalBinCount <= 0) return false;
	if (provisionalBinBoundaries.size() != provisionalBinCount + 1) return false;
	if (provisionalBinWeights.size() != provisionalBinCount + 1) return false;

	std::vector<float> readBinBoundaries;
	for (const auto& bound : provisionalBinBoundaries)
	{
		float provisionalBound;
		if (!jget(provisionalBound, bound)) return false;
		readBinBoundaries.push_back(provisionalBound);
		if (provisionalBound <= readBinBoundaries.back()) return false;
	}

	std::vector<float> readBinWeights;
	for (const auto& weight : provisionalBinWeights)
	{
		float provisionalWeight;
		if (!jget(provisionalWeight, weight)) return false;
		readBinWeights.push_back(provisionalWeight);
		if (provisionalWeight <= 0.f) return false;
	}

	binBoundaries.clear();
	binWeights.clear();
	binCount = provisionalBinCount;
	for (float bound : readBinBoundaries)
	{
		binBoundaries.push_back(bound);
	}
	for (float weight : readBinWeights)
	{
		binWeights.push_back(weight);
	}


	mDistribution = std::piecewise_constant_distribution<float>(binBoundaries.begin(), binBoundaries.end(), binWeights.begin());

	return true;
}


