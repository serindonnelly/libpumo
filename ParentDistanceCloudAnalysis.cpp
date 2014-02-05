#include "ParentDistanceCloudAnalysis.h"
#include "ForestAnalysis.h"
#include "AxesAnalysis.h"
#include "common.h"

ParentDistanceCloudAnalysis::ParentDistanceCloudAnalysis()
{
}


ParentDistanceCloudAnalysis::~ParentDistanceCloudAnalysis()
{
}
/***********************************************************************
 *  Method: ParentDistanceCloudAnalysis::serialise
 *  Params: picojson::value &v
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
ParentDistanceCloudAnalysis::serialise(picojson::value &v) const
{
	picojson::array va;
	for (const auto& d : data)
	{
		picojson::object vd;
		vd["a"] = picojson::value(d.angle);
		vd["d"] = picojson::value(d.maxDistance);
		vd["p"] = picojson::value(d.parentAngle);
		va.push_back(picojson::value(vd));
	}
	picojson::object vo;
	vo["data"] = picojson::value(va);
	v = picojson::value(vo);
	return true;
}


/***********************************************************************
 *  Method: ParentDistanceCloudAnalysis::deserialise
 *  Params: const picojson::value &v
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
ParentDistanceCloudAnalysis::deserialise(const picojson::value &v)
{
	return false;
}


/***********************************************************************
 *  Method: ParentDistanceCloudAnalysis::updateImpl
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
ParentDistanceCloudAnalysis::updateImpl()
{
	const Forest* f = ((ForestAnalysis*)inputs[0])->getForest();
	const vec pf = ((AxesAnalysis*)inputs[1])->getPF();
	const std::map<int,float>& distances = ((MaxDistanceDistributionAnalysis*)inputs[2])->distances;
	std::vector<Point> sample;
	std::vector<int> sampleIDs;
	std::vector<float> sampleRatios;
	f->samplePoints(sample,100000,&sampleIDs,&sampleRatios);
	for (unsigned int i = 0; i < sample.size(); i++)
	{
		const Node* n = f->getNode(sampleIDs[i]);
		if (n->isRoot() || !n->isDendrite())
			continue;
		const Node* parentNode = n->getParent();
		if (parentNode->isRoot() || !parentNode->isDendrite())
			continue;
		Displacement seg = n->getSegment()->getVector();
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

		Segment* parentSeg = parentNode->getSegment();
		float cosParent = pf.cosine(parentSeg->getVector());
		if (cosParent < -1.0001f || cosParent > 1.0001f)
			continue;
		// avoid domain errors
		if (cosParent < -1.f)
			cosParent = -1.f;
		else if (cosParent > 1.f)
			cosParent = 1.f;
		float parentAngle = RAD_TO_DEG*acos(cosParent);

		float distance = baseDistance + seg.norm()*(1.f - sampleRatios[i]);
		data.push_back(ParentDistanceAngle(parentAngle, distance, angle));
	}
}


