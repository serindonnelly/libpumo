#include "HeightDistributionAnalysis.h"
#include "ForestAnalysis.h"
#include "AxesAnalysis.h"
#include "common.h"


HeightDistributionAnalysis::HeightDistributionAnalysis()
{
}


HeightDistributionAnalysis::~HeightDistributionAnalysis()
{
}
/***********************************************************************
 *  Method: HeightDistributionAnalysis::selectDistribution
 *  Params: const Node *n, int &selection
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
HeightDistributionAnalysis::selectDistribution(const Node *n, int &selection) const
{
	return false;
	//height dependent distribution is unworkable
}


/***********************************************************************
 *  Method: HeightDistributionAnalysis::collectSample
 *  Params: std::vector<float> &samplesX, std::vector<float> &samplesY, std::vector<float> &sampleWeights
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
HeightDistributionAnalysis::collectSample(
std::vector<float> &samplesX,
std::vector<float> &samplesY,
std::vector<float> &sampleWeights) const
{
	const Forest* f = ((ForestAnalysis*)inputs[0])->getForest();
	const vecN pf = ((AxesAnalysis*)inputs[1])->getPF();
	std::vector<Point> samples;
	std::vector<int> sampleIDs;
	std::vector<float> sampleRatios;
	f->samplePoints(samples, 5000, &sampleIDs, &sampleRatios); // TODO factor out magic number of samples
	for (unsigned int i = 0; i < samples.size() && i < sampleIDs.size() && sampleRatios.size(); i++)
	{
		float angle = RAD_TO_DEG*acos(pf.cosine(f->getSegment(sampleIDs[i])->getVector()));
		float height = pf.dot(samples[i]);
		samplesX.push_back(angle);
		samplesY.push_back(height);
		sampleWeights.push_back(1.f-sampleRatios[i]);
		// TODO collect point distance along segment.
		// needs alteration to Forest or comparison between
		// norm(sample-proximal node) and segment length.
	}
}
