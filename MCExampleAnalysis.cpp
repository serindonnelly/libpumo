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
	int nodeCount = f->getGraph().size();
	for (int i = 0; i < mTreeCount; i++)
	{
		auto angleGenerator = std::bind(&DistributionAnalysis::generateAngle, dist, std::placeholders::_1, std::placeholders::_2);
		int alteredNodeCount;
		Forest* ff = f->generateForest(angleGenerator, pf, alteredNodeCount);
		examples.push_back(ff);
		WidthCalculator wc(ff, pf);
		widths.push_back(wc.getWidth());
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
	vo["original"] = picojson::value(voriginal);
	picojson::array vexamples;
	for (int i = 0; i < mTreeCount; i++)
	{
		picojson::object vtree;
		vtree["width"] = widths[i].serialise();
		vtree["swc"] = picojson::value(examples[i]->getSWCString());
		vtree["altered"] = picojson::value(alteredFractions[i]);
		vexamples.push_back(picojson::value(vtree));
	}
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
	return false;
}


