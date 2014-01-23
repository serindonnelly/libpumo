#include "MCExampleAnalysis.h"
#include "ForestAnalysis.h"
#include "AxesAnalysis.h"
#include "DistributionAnalysis.h"
#include "common.h"


MCExampleAnalysis::MCExampleAnalysis()
{
}


MCExampleAnalysis::~MCExampleAnalysis()
{
}

/***********************************************************************
 *  Method: MCExampleAnalysis::updateImpl
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
MCExampleAnalysis::updateImpl()
{
	const Forest* f = ((ForestAnalysis*)inputs[0])->getForest();
	vecN pf = ((AxesAnalysis*)inputs[1])->getPF();
	DistributionAnalysis* dist = (DistributionAnalysis*)inputs[2];
	original = new Forest(*f);
	originalWidth = WidthCalculator(f,pf).getWidth();
	originalLength = f->getTotalLength();
	int nodeCount = f->getGraph().size();
	for (int i = 0; i < mTreeCount; i++)
	{
		auto angleGenerator = std::bind(&DistributionAnalysis::generateAngle, dist, std::placeholders::_1, std::placeholders::_2);
		int alteredNodeCount;
		Forest* ff = f->generateForest(angleGenerator, pf, alteredNodeCount);
		examples.push_back(ff);
		WidthCalculator wc(ff, pf);
		widths.push_back(wc.getWidth());
		lengths.push_back(ff->getTotalLength());
		alteredFractions.push_back(((float)alteredNodeCount) / nodeCount);
	}
}


/***********************************************************************
 *  Method: MCExampleAnalysis::serialise
 *  Params: picojson::value &v
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
MCExampleAnalysis::serialise(picojson::value &v) const
{
	picojson::object vo;
	picojson::object voriginal;
	voriginal["width"] = originalWidth.serialise();
	voriginal["swc"] = picojson::value(original->getSWCString());
	voriginal["length"] = picojson::value(originalLength);
	vo["original"] = picojson::value(voriginal);
	picojson::array vexamples;
	for (int i = 0; i < mTreeCount; i++)
	{
		picojson::object vtree;
		vtree["width"] = widths[i].serialise();
		vtree["swc"] = picojson::value(examples[i]->getSWCString());
		vtree["altered"] = picojson::value(alteredFractions[i]);
		vtree["length"] = picojson::value(lengths[i]);
		vexamples.push_back(picojson::value(vtree));
	}
	vo["examples"] = picojson::value(vexamples);
	v = picojson::value(vo);
	return true;
}


/***********************************************************************
 *  Method: MCExampleAnalysis::deserialise
 *  Params: const picojson::value &v
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
MCExampleAnalysis::deserialise(const picojson::value &v)
{
	picojson::object voriginal;
	picojson::array vexamples;
	if (!jat(voriginal, v, "original")) return false;
	if (!jat(vexamples, v, "examples")) return false;

	WidthGroup wgo;
	if (!jat(wgo, picojson::value(voriginal), "width")) return false;
	float lo;
	if (!jat(lo, picojson::value(voriginal), "length")) return false;
	std::string swco;
	if (!jat(swco, picojson::value(voriginal), "swc")) return false;

	for (const auto& vv : vexamples)
	{
		WidthGroup wge;
		if (!jat(wge, vv, "width")) return false;
		float alterede;
		if (!jat(alterede, vv, "altered")) return false;
		float lengthe;
		if (!jat(lengthe, vv, "length")) return false;
		std::string swce;
		if (!jat(swce, vv, "swc")) return false;
	}
	// not as safe as other deserialisers
	original = new Forest();
	original->readSWC(std::stringstream(swco), false);
	originalLength = lo;
	originalWidth = wgo;
	for (const auto& vv : vexamples)
	{
		WidthGroup wge;
		jat(wge, vv, "width");
		widths.push_back(wge);

		float alterede;
		jat(alterede, vv, "altered");
		alteredFractions.push_back(alterede);

		float lengthe;
		jat(lengthe, vv, "length");
		lengths.push_back(lengthe);

		std::string swce;
		jat(swce, vv, "swc");
		Forest* nf = new Forest();
		nf->readSWC(std::stringstream(swce), false);
		examples.push_back(nf);
	}

	return true;
}


