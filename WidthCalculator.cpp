#include "WidthCalculator.h"
#include <vector>

WidthCalculator::WidthCalculator()
{
}


//WidthCalculator::~WidthCalculator()
//{
//}

/***********************************************************************
 *  Method: WidthCalculator::calculate
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
WidthCalculator::calculate()
{
	calculateIQ();
	calculateSD();
	calculateMinMax();
}


/***********************************************************************
 *  Method: WidthCalculator::addSegment
 *  Params: float low, float high, float realLength
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
WidthCalculator::addSegment(float low, float high, float realLength)
{
	float llow, hhigh;
	if (low > high)
	{
		llow = high; hhigh = low;
	}
	else
	{
		llow = low; hhigh = high;
	}
	if (hhigh != llow)
	{
		boundsToDifferences[std::make_pair(llow, false)] += realLength / (hhigh - llow);
		boundsToDifferences[std::make_pair(hhigh, false)] -= realLength / (hhigh - llow);
	}
	else
		boundsToDifferences[std::make_pair(llow, true)] += realLength;
	weightedSumHeight += 0.5f*(hhigh + llow)*realLength;
	totalLengthDirect += realLength;
	lowHighLengths.push_back(std::make_pair(std::make_pair(llow, hhigh), realLength));
}


/***********************************************************************
 *  Method: WidthCalculator::calculateIQ
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
WidthCalculator::calculateIQ()
{
	float currentTotal = 0.f;
	float currentHeight = 0.f;
	float currentDensity = 0.f;
	// iterate over boundary heights defined by segment start and end points,
	// accumulating floating point error (which will be recorded) while summing,
	// and watch for when we pass the lower and upper quartiles.
	// depending on magnitude of recorded errors, consider switching
	// to a more precise algorithm like Kahan summation.
	for (const auto& boundPointDifference : boundsToDifferences)
	{
		float nextHeight = boundPointDifference.first.first;
		bool isPoint = boundPointDifference.first.second;
		float difference = boundPointDifference.second;
		float nextTotal;
		if (isPoint)
		{
			nextTotal = currentTotal + difference;
			if (currentTotal < totalLengthDirect / 4.f &&
				nextTotal >= totalLengthDirect / 4.f)
			{
				lowerQuartile = nextHeight;
			}
			else if (currentTotal < 3.f*totalLengthDirect / 4.f &&
				nextTotal >= 3.f*totalLengthDirect / 4.f)
			{
				upperQuartile = nextHeight;
			}

		}
		else
		{
			float heightDifference = nextHeight - currentHeight;
			nextTotal = currentTotal + heightDifference*currentDensity;
			currentDensity += difference;
			if (currentTotal < totalLengthDirect / 4.f &&
				nextTotal >= totalLengthDirect / 4.f)
			{
				float ratio = (totalLengthDirect/4.f - currentTotal) / (nextTotal - currentTotal);
				lowerQuartile = currentHeight + ratio*(nextHeight - currentHeight);
			}
			else if (currentTotal < 3.f*totalLengthDirect / 4.f &&
				nextTotal >= 3.f*totalLengthDirect / 4.f)
			{
				float ratio = (3.f*totalLengthDirect / 4.f - currentTotal) / (nextTotal - currentTotal);
				upperQuartile = currentHeight + ratio*(nextHeight - currentHeight);
			}
		}
		currentHeight = nextHeight;
		currentTotal = nextTotal;
	}
	totalLengthError = currentTotal - totalLengthDirect;
	finalDifferenceError = currentDensity;
}


/***********************************************************************
 *  Method: WidthCalculator::calculateSD
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
WidthCalculator::calculateSD()
{
	calculateMean();
	float variance = 0.f;
	for (const auto& lhl : lowHighLengths)
	{
		float low = lhl.first.first;
		float high = lhl.first.second;
		float length = lhl.second;
		if (mean > low && mean < high)
		{
			float ratio = (mean - low) / (high - low);
			float contributionsqrt = 0.5f*(mean - low)*ratio*length;
			variance += contributionsqrt*contributionsqrt;
			contributionsqrt = 0.5f*(high - mean)*(1.f - ratio)*length;
			variance += contributionsqrt*contributionsqrt;
		}
		else if (mean > high)
		{
			float contributionsqrt = 0.5f*(2.f*mean - high - low)*length;
			variance += contributionsqrt*contributionsqrt;
		}
		else if (mean > high)
		{
			float contributionsqrt = 0.5f*(high + low - 2.f*mean)*length;
			variance += contributionsqrt*contributionsqrt;
		}
	}
	variance = variance / totalLengthDirect;
	standardDeviation = sqrt(variance);
}


/***********************************************************************
 *  Method: WidthCalculator::calculateMinMax
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
WidthCalculator::calculateMinMax()
{
	min = (*boundsToDifferences.begin()).first.first;
	max = (*boundsToDifferences.rbegin()).first.first;
}


/***********************************************************************
 *  Method: WidthCalculator::calculateMean
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
WidthCalculator::calculateMean()
{
	mean = weightedSumHeight / totalLengthDirect;
}


/***********************************************************************
 *  Method: WidthCalculator::WidthCalculator
 *  Params: Forest *f
 * Effects: 
 ***********************************************************************/
WidthCalculator::WidthCalculator(const Forest *f, vecN pf)
{
	for (const auto& n : f->getGraph())
	{
		if (n.second->isRoot())
			continue;
		const Segment* seg = n.second->getSegment();
		//want to get start and end points as quickly as posible here, ie. without a map lookup
		Point parentPos = seg->getProximalNode()->getPosition();
		Point nodePos = n.second->getPosition();
		addSegment(pf.dot(parentPos), pf.dot(nodePos), seg->getVector().norm());
	}
	calculate();
}




