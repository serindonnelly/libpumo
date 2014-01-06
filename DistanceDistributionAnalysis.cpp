#include "DistanceDistributionAnalysis.h"
#include "ForestAnalysis.h"
#include "AxesAnalysis.h"
#include "common.h"


DistanceDistributionAnalysis::DistanceDistributionAnalysis()
{
}


DistanceDistributionAnalysis::~DistanceDistributionAnalysis()
{
}
/***********************************************************************
 *  Method: DistanceDistributionAnalysis::selectDistribution
 *  Params: const Node *n, int &selection
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
DistanceDistributionAnalysis::selectDistribution(const Node *n, int &selection) const
{
	if (n->isRoot())
		return false;
	std::vector<float> binWeights;
	binWeights.resize(mHistogram.getBinCountY());
	const int sampleCount = 10;
	std::list<float> nodeDistances = distances.at(n->getID());
	for (float baseDistance : nodeDistances)
	{
		float length = n->getSegment()->getVector().norm();
		for (float distance = baseDistance + length / (2 * sampleCount);
			distance < baseDistance + length;
			distance += length / (sampleCount))
		{
			int bin = mHistogram.sampleInsertBinY(distance);
			float weight = 1.f / nodeDistances.size();
			binWeights[bin] += weight/binTotals[bin];
		}
	}
	//proper initialisation of std::discrete_distribution not supported
	std::discrete_distribution<int> distanceBinSelector(mHistogram.getBinCountY(), 0, mHistogram.getBinCountY(), [&](int y)->float {return binWeights[y]; });
	selection = distanceBinSelector(gen);
	return true;
}


/***********************************************************************
 *  Method: DistanceDistributionAnalysis::collectSample
 *  Params: std::vector<float> &samplesX, std::vector<float> &samplesY, std::vector<float> &sampleWeights
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
DistanceDistributionAnalysis::collectSample(
std::vector<float> &samplesX,
std::vector<float> &samplesY,
std::vector<float> &sampleWeights) const
{


	const Forest* f = ((ForestAnalysis*)inputs[0])->getForest();

	const vecN pf = ((AxesAnalysis*)inputs[1])->getPF();
	std::vector<Point> samples;
	std::vector<int> sampleIDs;
	std::vector<float> sampleRatios;
	f->samplePoints(samples, 5000000, &sampleIDs, &sampleRatios); // TODO factor out magic number of samples
	for (unsigned int i = 0; i < samples.size() && i < sampleIDs.size() && i < sampleRatios.size(); i++)
	{
		Displacement seg = f->getSegment(sampleIDs[i])->getVector();
		float angle = RAD_TO_DEG*acos(pf.cosine(seg));
		for (float baseDistance : distances.at(sampleIDs[i]))
		{
			float distance = baseDistance + seg.norm()*(1.f - sampleRatios[i]);
			samplesX.push_back(angle);
			samplesY.push_back(distance);
			sampleWeights.push_back(1.f / distances.at(sampleIDs[i]).size());
		}
	}
}


/***********************************************************************
 *  Method: DistanceDistributionAnalysis::annotateDistances
 *  Params: const Forest *f
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
DistanceDistributionAnalysis::annotateDistances(const Forest *f)
{
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
			distances[n->getID()].push_back(dist);
			dist += n->getSegment()->getVector().norm();
			n = n->getParent();
		}
	}

}


/***********************************************************************
 *  Method: DistanceDistributionAnalysis::updateImpl
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
DistanceDistributionAnalysis::updateImpl()
{
	annotateDistances(((ForestAnalysis*)inputs[0])->getForest());
	JointDistributionAnalysis::updateImpl();
	// sum rows
	for (int y = 0; y < mHistogram.getBinCountY(); y++)
	{
		for (int x = 0; x < mHistogram.getBinCountX(); x++)
		{
			binTotals[y] += mHistogram(x, y);
		}
	}
}


