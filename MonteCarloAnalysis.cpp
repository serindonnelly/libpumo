#include "MonteCarloAnalysis.h"
#include "ForestAnalysis.h"
#include "AxesAnalysis.h"
#include "DistributionAnalysis.h"
#include "common.h"


MonteCarloAnalysis::MonteCarloAnalysis()
{
}


MonteCarloAnalysis::~MonteCarloAnalysis()
{
}



/***********************************************************************
 *  Method: MonteCarloAnalysis::updateImpl
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
MonteCarloAnalysis::updateImpl()
{
	const Forest* f = ((ForestAnalysis*)inputs[0])->getForest();
	vecN pf = ((AxesAnalysis*)inputs[1])->getPF();
	originalWidth = f->getWidth(pf);
	DistributionAnalysis* dist = (DistributionAnalysis*)inputs[2];
	widths.clear();
	for (int i = 0; i < mTreeCount; i++)
	{
		auto angleGenerator = std::bind(&DistributionAnalysis::generateAngle, dist,std::placeholders::_1,std::placeholders::_2);
		Forest* ff = f->generateForest(angleGenerator, pf);
		widths.push_back(ff->getWidth(pf));
		delete ff;
	}
	//get mean, std of widths
}


/***********************************************************************
 *  Method: MonteCarloAnalysis::serialise
 *  Params: picojson::value &v
 * Returns: void
 * Effects: 
 ***********************************************************************/
bool
MonteCarloAnalysis::serialise(picojson::value &v) const
{
	picojson::object vo;
	vo["original_width"] = picojson::value(originalWidth);
	picojson::array va;
	for (float width : widths)
	{
		va.push_back(picojson::value(width));
	}
	vo["generated_widths"] = picojson::value(va);
	v = picojson::value(vo);
	return true;
}


/***********************************************************************
 *  Method: MonteCarloAnalysis::deserialise
 *  Params: const picojson::value &v
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
MonteCarloAnalysis::deserialise(const picojson::value &v)
{
	float provisionalOriginalWidth;
	if (!jat(provisionalOriginalWidth, v, "original_width"))
		return false;

	picojson::array provisionalWidths;
	if (!jat(provisionalWidths, v, "generated_widths"))
		return false;

	if (provisionalWidths.size() != mTreeCount)
		return false;
	for (const auto& width : provisionalWidths)
	{
		float readWidth;
		if (!jget(readWidth, width)) return false;
	}

	widths.clear();
	originalWidth = provisionalOriginalWidth;
	for (const auto& width : provisionalWidths)
	{
		float readWidth;
		jget(readWidth, width);
		widths.push_back(readWidth);
	}
	return true;
}


