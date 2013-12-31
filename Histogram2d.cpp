#include <math.h>
#include "Histogram2d.h"


Histogram2d::Histogram2d()
{

}


Histogram2d::~Histogram2d()
{
}
/***********************************************************************
 *  Method: Histogram2d::operator()
 *  Params: int x, int y
 * Returns: float
 * Effects: 
 ***********************************************************************/
float
Histogram2d::operator()(int x, int y) const
{
	return mHistogram(x, y);
}


/***********************************************************************
 *  Method: Histogram2d::insertSample
 *  Params: float x, float y, float weight
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
Histogram2d::insertSample(float x, float y, float weight)
{
	if (getMinX() > x || getMaxX() < x)
		return false;
	if (getMinY() > y || getMaxY() < y)
		return false;
	int indexX, indexY;
	indexX = (int)floor((float)binCountX * (x - getMinX()) / (getMaxX() - getMinX()));
	indexY = (int)floor((float)binCountY * (x - getMinY()) / (getMaxY() - getMinY()));
	if (indexX == binCountX)
		indexX--;
	if (indexY == binCountY)
		indexY--;
	mHistogram(indexX, indexY) += weight;
	return true;
}


/***********************************************************************
 *  Method: Histogram2d::setBinCountX
 *  Params: int count
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Histogram2d::setBinCountX(int count)
{
	binCountX = count;
	zero();
}


/***********************************************************************
 *  Method: Histogram2d::setBinCountY
 *  Params: int count
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Histogram2d::setBinCountY(int count)
{
	binCountY = count;
	zero();
}


/***********************************************************************
 *  Method: Histogram2d::setRangeX
 *  Params: float minX, float maxX
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Histogram2d::setRangeX(float minX, float maxX)
{
	binBoundariesX.clear();
	binBoundariesX.push_back(minX);
	binBoundariesX.push_back(maxX);
	zero();
}


/***********************************************************************
 *  Method: Histogram2d::setRangeY
 *  Params: float minY, float maxY
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Histogram2d::setRangeY(float minY, float maxY)
{
	binBoundariesY.clear();
	binBoundariesY.push_back(minY);
	binBoundariesY.push_back(maxY);
	zero();
}


/***********************************************************************
 *  Method: Histogram2d::guessRangeX
 *  Params: const std::vector<float> &samples
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Histogram2d::guessRangeX(const std::vector<float> &samples)
{
	float minX = samples[0];
	float maxX = samples[0];
	for (const float& s : samples)
	{
		if (s < minX)
			minX = s;
		if (s > maxX)
			maxX = s;
	}
	setRangeX(minX, maxX);
}


/***********************************************************************
 *  Method: Histogram2d::guessRangeY
 *  Params: const std::vector<float> &samples
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Histogram2d::guessRangeY(const std::vector<float> &samples)
{
	float minY = samples[0];
	float maxY = samples[0];
	for (const float& s : samples)
	{
		if (s < minY)
			minY = s;
		if (s > maxY)
			maxY = s;
	}
	setRangeY(minY, maxY);
}


/***********************************************************************
 *  Method: Histogram2d::zero
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Histogram2d::zero()
{
	mHistogram.resize(binCountX, binCountY);
	mHistogram.fill(0); // check this works

	float minX = getMinX();
	float maxX = getMaxX();
	binBoundariesX.clear();
	for (int i = 0; i <= binCountX; i++)
	{
		//risk of floating point errors in maxX, maxY
		binBoundariesX.push_back(minX + (float)i*((maxX - minX) / binCountX));
	}

	float minY = getMinY();
	float maxY = getMaxY();
	binBoundariesY.clear();
	for (int i = 0; i <= binCountY; i++)
	{
		//risk of floating point errors in maxX, maxY
		binBoundariesY.push_back(minY + (float)i*((maxY - minY) / binCountY));
	}

}


/***********************************************************************
 *  Method: Histogram2d::getminX
 *  Params: 
 * Returns: float
 * Effects: 
 ***********************************************************************/
float
Histogram2d::getMinX() const
{
	return binBoundariesX.front();
}


/***********************************************************************
 *  Method: Histogram2d::getmaxX
 *  Params: 
 * Returns: float
 * Effects: 
 ***********************************************************************/
float
Histogram2d::getMaxX() const
{
	return binBoundariesX.back();
}


/***********************************************************************
 *  Method: Histogram2d::getminY
 *  Params: 
 * Returns: float
 * Effects: 
 ***********************************************************************/
float
Histogram2d::getMinY() const
{
	return binBoundariesY.front();
}


/***********************************************************************
 *  Method: Histogram2d::getmaxY
 *  Params: 
 * Returns: float
 * Effects: 
 ***********************************************************************/
float
Histogram2d::getMaxY() const
{
	return binBoundariesY.back();
}


