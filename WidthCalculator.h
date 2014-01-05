#pragma once
#include <vector>
#include <map>

class WidthCalculator
{
public:
	WidthCalculator();
	~WidthCalculator();
	void addSegment(float low, float high, float realLength);
	void calculate();
private:
	std::map<std::pair<float,bool>, float> boundsToDifferences;
	std::vector < std::pair<std::pair<float, float>, float>> lowHighLengths;
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

