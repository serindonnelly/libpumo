#include "MaxDistanceDistributionAnalysis.h"
#include "ForestAnalysis.h"
#include "AxesAnalysis.h"
#include "common.h"


MaxDistanceDistributionAnalysis::MaxDistanceDistributionAnalysis()
{
}


MaxDistanceDistributionAnalysis::~MaxDistanceDistributionAnalysis()
{
}

/***********************************************************************
 *  Method: MaxDistanceDistributionAnalysis::selectDistribution
 *  Params: const Node *n, int &selection
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
MaxDistanceDistributionAnalysis::selectDistribution(const Node *n, int &selection) const
{
	if (n->isRoot())
		return false;
	std::vector<float> binWeights;
	binWeights.resize(mHistogram.getBinCountY());
	const int sampleCount = 10;
	float baseDistance = distances.at(n->getID());
	float length = n->getSegment()->getVector().norm();
	for (float distance = baseDistance + length / (2 * sampleCount);
		distance < baseDistance + length;
		distance += length / (sampleCount))
	{
		assert(baseDistance + length <= mMaxDistance);
		int bin = mHistogram.sampleInsertBinY(distance);
		binWeights[bin] += 1.f / binTotals[bin];
	}
	//proper initialisation of std::discrete_distribution not supported
	std::discrete_distribution<int> distanceBinSelector(mHistogram.getBinCountY(), 0, mHistogram.getBinCountY(), [&](int y)->float {return binWeights[y]; });
	selection = distanceBinSelector(gen);
	return true;
}


/***********************************************************************
 *  Method: MaxDistanceDistributionAnalysis::collectSample
 *  Params: std::vector<float> &samplesX, std::vector<float> &samplesY, std::vector<float> &sampleWeights
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
MaxDistanceDistributionAnalysis::collectSample(std::vector<float> &samplesX, std::vector<float> &samplesY, std::vector<float> &sampleWeights) const
{
	const Forest* f = ((ForestAnalysis*)inputs[0])->getForest();

	const vecN pf = ((AxesAnalysis*)inputs[1])->getPF();
	std::vector<Point> samples;
	std::vector<int> sampleIDs;
	std::vector<float> sampleRatios;
	f->samplePoints(samples, 1000000, &sampleIDs, &sampleRatios); // TODO factor out magic number of samples
	for (unsigned int i = 0; i < samples.size() && i < sampleIDs.size() && i < sampleRatios.size(); i++)
	{
		Displacement seg = f->getSegment(sampleIDs[i])->getVector();
		float segCosine = pf.cosine(seg);

		if (segCosine < -1.0001f || segCosine > 1.0001f)
			continue;
		// avoid domain errors
		if (segCosine < -1.f)
			segCosine = -1.f;
		else if (segCosine > 1.f)
			segCosine = 1.f;
		float angle = RAD_TO_DEG*acos(segCosine);
		float baseDistance = distances.at(sampleIDs[i]);
		float distance = baseDistance + seg.norm()*(1.f - sampleRatios[i]);
		samplesX.push_back(angle);
		samplesY.push_back(distance);
		sampleWeights.push_back(1.f);
		
	}
}

/***********************************************************************
 *  Method: MaxDistanceDistributionAnalysis::annotateDistances
 *  Params: const Forest *f
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
MaxDistanceDistributionAnalysis::annotateDistances(const Forest *f)
{
	mMaxDistance = 0.f;
	std::list<int> terminals;
	for (const auto& n : f->getGraph())
	{
		if (n.second->numChildren() == 0)
			terminals.push_back(n.first);
	}
	for (int t : terminals)
	{
		const Node* n = f->getNode(t);
		float dist = 0.f;
		while (n->getParentID() != -1)
		{
			distances[n->getID()] = std::max(distances[n->getID()],dist);
			dist += n->getSegment()->getVector().norm();
			if (dist > mMaxDistance)
				mMaxDistance = dist;
			n = n->getParent();
		}
	}
}


/***********************************************************************
 *  Method: MaxDistanceDistributionAnalysis::serialiseAdditional
 *  Params: picojson::value &v
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
MaxDistanceDistributionAnalysis::serialiseAdditional(picojson::value &v) const
{
	picojson::object vo;
	picojson::object vd;
	for (const auto& dd : distances)
	{
		float dist = dd.second;
		vd[std::to_string(dd.first)] = picojson::value(dist);
	}
	vo["distances"] = picojson::value(vd);
	vo["max_distance"] = picojson::value(mMaxDistance);
	v = picojson::value(vo);
	return true;
}


/***********************************************************************
 *  Method: MaxDistanceDistributionAnalysis::deserialiseAdditional
 *  Params: const picojson::value &v
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
MaxDistanceDistributionAnalysis::deserialiseAdditional(const picojson::value &v)
{
	picojson::object vd;
	if (!jat(vd, v, "distances")) return false;
	float provisionalMaxDistance;
	if (!jat(provisionalMaxDistance, v, "max_distance")) return false;
	std::map<int, float> provisionalDistances;
	for (const auto& indexDist : vd)
	{
		int provisionalIndex = std::stoi(indexDist.first);
		if (provisionalDistances.find(provisionalIndex) != provisionalDistances.end()) return false;

		float pDist;
		if (!jget(pDist, indexDist.second)) return false;
		provisionalDistances[provisionalIndex] = pDist;
	}

	mMaxDistance = provisionalMaxDistance;
	distances = provisionalDistances;
	return true;
}


/***********************************************************************
*  Method: MaxDistanceDistributionAnalysis::updateImpl
*  Params:
* Returns: void
* Effects:
***********************************************************************/
void
MaxDistanceDistributionAnalysis::updateImpl()
{
	annotateDistances(((ForestAnalysis*)inputs[0])->getForest());
	JointDistributionAnalysis::updateImpl();
}
