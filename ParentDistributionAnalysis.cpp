#include "ParentDistributionAnalysis.h"
#include "ForestAnalysis.h"
#include "AxesAnalysis.h"
#include "common.h"


ParentDistributionAnalysis::ParentDistributionAnalysis()
{
}


ParentDistributionAnalysis::~ParentDistributionAnalysis()
{
}
/***********************************************************************
 *  Method: ParentDistributionAnalysis::selectDistribution
 *  Params: const Node *n, int &selection
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
ParentDistributionAnalysis::selectDistribution(const Node *n, int &selection) const
{
	if (n->isRoot())
		return false;
	if (n->getParent()->isRoot())
		return false;
	vecN pf = ((AxesAnalysis*)inputs[1])->getPF();
	float parentCos = pf.cosine(n->getParent()->getSegment()->getVector());
	//assert(parentCos >= -1.f && parentCos <= 1.f);
	if (parentCos < -1.0001f || parentCos > 1.0001f)
		return false;
	// avoid domain errors
	if (parentCos < -1.f)
		parentCos = -1.f;
	else if (parentCos > 1.f)
		parentCos = 1.f;
	float parentAngle = RAD_TO_DEG*acos(parentCos);
	int indexY = mHistogram.sampleInsertBinY(parentAngle); // fails when segment is too close to antiparallel with pf
	if (indexY == -1)
		return false;
	selection = indexY;
	return true;
}


/***********************************************************************
 *  Method: ParentDistributionAnalysis::collectSample
 *  Params: std::vector<float> &samplesX, std::vector<float> &samplesY, std::vector<float> &sampleWeights
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
ParentDistributionAnalysis::collectSample(std::vector<float> &samplesX, std::vector<float> &samplesY, std::vector<float> &sampleWeights) const
{
	const Forest* f = ((ForestAnalysis*)inputs[0])->getForest();
	const vecN pf = ((AxesAnalysis*)inputs[1])->getPF();
	for (const auto& n : f->getGraph())
	{
		if (!n.second->isRoot())
		{
			if (!n.second->getParent()->isRoot())
			{
				Segment* seg = n.second->getSegment();
				Segment* parentSeg = n.second->getParent()->getSegment();
				samplesX.push_back(RAD_TO_DEG*acos(pf.cosine(seg->getVector())));
				samplesY.push_back(RAD_TO_DEG*acos(pf.cosine(parentSeg->getVector())));
				sampleWeights.push_back(seg->getVector().norm());
			}
		}
	}
}


