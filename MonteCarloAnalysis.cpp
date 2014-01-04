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
	int nodeCount = f->getGraph().size();
	for (int i = 0; i < mTreeCount; i++)
	{
		auto angleGenerator = std::bind(&DistributionAnalysis::generateAngle, dist,std::placeholders::_1,std::placeholders::_2);
		int alteredNodeCount;
		Forest* ff = f->generateForest(angleGenerator, pf, alteredNodeCount);
		widths.push_back(ff->getWidth(pf));
		alteredFractions.push_back(((float)alteredNodeCount) / nodeCount);
		delete ff;
	}
	mean = 0.f;
	for (const auto width : widths)
	{
		mean += width;
	}
	mean /= mTreeCount;
	SD = 0.f;
	for (const auto width : widths)
	{
		SD += (width - mean)*(width - mean);
	}
	SD /= mTreeCount; // mTreeCount-1?
	SD = sqrt(SD);
	if (SD != 0.f)
	{
		discrepancy = (mean - originalWidth) / SD;
	}
	else
	{
		discrepancy = 0.f;
	}
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
	vo["mean"] = picojson::value(mean);
	vo["standard_deviation"] = picojson::value(SD);
	vo["discrepancy"] = picojson::value(discrepancy);

	picojson::array vaw;
	for (unsigned int i = 0; i < widths.size() && i < alteredFractions.size(); i++)
	{
		picojson::object vp;
		vp["width"] = picojson::value(widths[i]);
		vp["fract"] = picojson::value(alteredFractions[i]);
		vaw.push_back(picojson::value(vp));
	}
	vo["generated_widths"] = picojson::value(vaw);

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
	float provisionalMean;
	if (!jat(provisionalMean, v, "mean"))
		return false;
	float provisionalSD;
	if (!jat(provisionalSD, v, "standard_deviation"))
		return false;
	float provisionalDiscrepancy;
	if (!jat(provisionalDiscrepancy, v, "discrepancy"))
		return false;

	picojson::array provisionalWidths;
	if (!jat(provisionalWidths, v, "generated_widths"))
		return false;

	if (provisionalWidths.size() != mTreeCount)
		return false;
	for (const auto& widthFract : provisionalWidths)
	{
		float readWidth;
		float readFract;
		if (!jat(readWidth, widthFract, "width")) return false;
		if (!jat(readFract, widthFract, "fract")) return false;
	}

	widths.clear();
	alteredFractions.clear();
	originalWidth = provisionalOriginalWidth;
	mean = provisionalMean;
	SD = provisionalSD;
	discrepancy = provisionalDiscrepancy;
	for (const auto& widthFract : provisionalWidths)
	{
		float readWidth;
		float readFract;
		jat(readWidth, widthFract, "width");
		jat(readFract, widthFract, "fract");
		widths.push_back(readWidth);
		alteredFractions.push_back(readFract);
	}
	return true;
}


