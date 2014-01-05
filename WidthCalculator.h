#pragma once
#include <vector>
#include <map>
#include "Forest.h"
#include "WidthGroup.h"


class WidthCalculator
{
public:
	WidthCalculator();
	WidthCalculator(const Forest* f, vecN pf);
	//~WidthCalculator();
	void addSegment(float low, float high, float realLength);
	void calculate();
	WidthGroup getWidth() const { return WidthGroup(
		getIQRange(),getStandardDeviation(),getAbsRange()); }
private:
	std::map<std::pair<float,bool>, float> boundsToDifferences;
	std::vector < std::pair<std::pair<float, float>, float>> lowHighLengths;
	float getIQRange() const { return upperQuartile - lowerQuartile; }
	float getStandardDeviation() const { return standardDeviation; }
	float getAbsRange() const { return max - min; }
	void calculateIQ();
	void calculateSD();
	void calculateMean();
	void calculateMinMax();
	//std::vector<std::pair<float,float>> boundsAndDifferences;
	float lowerQuartile;
	float upperQuartile;
	float weightedSumHeight;
	float mean;
	float standardDeviation;
	float min;
	float max;
	float totalLengthDirect;
	float totalLengthError;
	float finalDifferenceError;
};

