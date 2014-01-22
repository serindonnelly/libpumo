#include "MonteCarloAnalysis.h"
#include "ForestAnalysis.h"
#include "AxesAnalysis.h"
#include "DistributionAnalysis.h"
#include "common.h"
#include "WidthCalculator.h"

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
	nodeCount = f->getGraph().size();
	std::cout << currentTimeString() << " Forest has " << nodeCount << " nodes" << std::endl;
	vecN pf = ((AxesAnalysis*)inputs[1])->getPF();
	WidthCalculator wco(f, pf);
	originalWidth = wco.getWidth();
	DistributionAnalysis* dist = (DistributionAnalysis*)inputs[2];
	widths.clear();
	int nodeCount = f->getGraph().size();
	for (int i = 0; i < mTreeCount; i++)
	{
		if (!(i % 20)) std::cout << i / 10;
		else if (!(i % 10)) std::cout << ".";
		auto angleGenerator = std::bind(&DistributionAnalysis::generateAngle, dist,std::placeholders::_1,std::placeholders::_2);
		int alteredNodeCount;
		Forest* ff = f->generateForest(angleGenerator, pf, alteredNodeCount);
		WidthCalculator wc(ff, pf);
		widths.push_back(wc.getWidth());
		alteredFractions.push_back(((float)alteredNodeCount) / nodeCount);
		assert(f->compare(ff));
		delete ff;
	}
	std::cout << std::endl;
	mean = { 0.f, 0.f, 0.f };
	for (const auto width : widths)
	{
		mean = mean + width;
	}
	mean = mean / mTreeCount;
	SD = { 0.f, 0.f, 0.f };
	for (const auto width : widths)
	{
		SD = SD + (width - mean)*(width - mean);
	}
	SD = SD / mTreeCount; // mTreeCount-1?
	SD = SD.sqrtWG();
	discrepancy = (mean - originalWidth) / SD;

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
	vo["original_width"] = originalWidth.serialise();
	vo["mean"] = mean.serialise();
	vo["standard_deviation"] = SD.serialise();
	vo["discrepancy"] = discrepancy.serialise();
	vo["node_count"] = picojson::value((double)nodeCount);

	picojson::array vaw;
	for (unsigned int i = 0; i < widths.size() && i < alteredFractions.size(); i++)
	{
		picojson::object vp;
		vp["width"] = widths[i].serialise();
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
	WidthGroup provisionalOriginalWidth(0,0,0);
	if (!jat(provisionalOriginalWidth, v, "original_width"))
	{
		return false;
		std::cout << "original width not found\n";
	}
	WidthGroup provisionalMean(0, 0, 0);
	if (!jat(provisionalMean, v, "mean"))
	{
		return false;
		std::cout << "mean not found\n";
	}
	WidthGroup provisionalSD(0, 0, 0);
	if (!jat(provisionalSD, v, "standard_deviation"))
	{
		return false;
		std::cout << "standard deviation not found\n";
	}
	WidthGroup provisionalDiscrepancy;
	if (!jat(provisionalDiscrepancy, v, "discrepancy"))
	{
		return false;
		std::cout << "discrepancy not found\n";
	}
	int provisionalNodeCount;
	if (!jat(provisionalNodeCount, v, "node_count"))
	{
		return false;
		std::cout << "node count not found\n";
	}

	picojson::array provisionalWidths;
	if (!jat(provisionalWidths, v, "generated_widths"))
	{
		return false;
		std::cout << "generated widths not found\n";
	}

	if (provisionalWidths.size() != mTreeCount)
	{
		return false;
		std::cout << "tree counts not equal\n";
	}
	for (const auto& widthFract : provisionalWidths)
	{
		WidthGroup readWidth;
		float readFract;
		if (!jat(readWidth, widthFract, "width"))
		{
			return false;
			std::cout << "width not found\n";
		}
		if (!jat(readFract, widthFract, "fract"))
		{
			return false;
			std::cout << "altered fraction not found\n";
		}
	}

	widths.clear();
	alteredFractions.clear();
	originalWidth = provisionalOriginalWidth;
	mean = provisionalMean;
	SD = provisionalSD;
	discrepancy = provisionalDiscrepancy;
	nodeCount = provisionalNodeCount;
	for (const auto& widthFract : provisionalWidths)
	{
		WidthGroup readWidth;
		float readFract;
		jat(readWidth, widthFract, "width");
		jat(readFract, widthFract, "fract");
		widths.push_back(readWidth);
		alteredFractions.push_back(readFract);
	}
	return true;
}


