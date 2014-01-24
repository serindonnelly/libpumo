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
 *  Method: Histogram2d::setBinCountX
 *  Params: int count
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Histogram2d::setBinCountX(int count)
{
	mBinCountX = count;
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
	mBinCountY = count;
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
	mMinX = minX;
	mMaxX = maxX;
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
	mMinY = minY;
	mMaxY = maxY;
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
	mHistogram.resize(mBinCountX, mBinCountY);
	mHistogram.fill(0); // check this works

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
	return mMinX;
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
	return mMaxX;
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
	return mMinY;
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
	return mMaxY;
}


/***********************************************************************
 *  Method: Histogram2d::getBinBoundariesX
 *  Params: 
 * Returns: const std::vector<float> &
 * Effects: 
 ***********************************************************************/
void 
Histogram2d::getBinBoundariesX(std::vector<float>& binBoundariesX) const
{
	for (int i = 0; i <= mBinCountX; i++)
	{
		//risk of floating point errors in maxX, maxY
		binBoundariesX.push_back(getBinBoundaryX(i));
	}
}


/***********************************************************************
 *  Method: Histogram2d::getBinBoundariesY
 *  Params: 
 * Returns: const std::vector<float> &
 * Effects: 
 ***********************************************************************/
void
Histogram2d::getBinBoundariesY(std::vector<float> & binBoundariesY) const
{
	for (int i = 0; i <= mBinCountY; i++)
	{
		//risk of floating point errors in maxX, maxY
		binBoundariesY.push_back(getBinBoundaryY(i));
	}
}


/***********************************************************************
 *  Method: Histogram2d::getBinBoundariesY
 *  Params: 
 * Returns: const std::vector<float> &
 * Effects: 
 ***********************************************************************/
float
Histogram2d::getBinBoundaryX(int i) const
{
	return mMinX + (float)i*((mMaxX - mMinX) / mBinCountX);
}


/***********************************************************************
 *  Method: Histogram2d::getBinBoundaryY
 *  Params: 
 * Returns: float
 * Effects: 
 ***********************************************************************/
float
Histogram2d::getBinBoundaryY(int i) const
{
	return mMinY + (float)i*((mMaxY - mMinY) / mBinCountY);
}


/***********************************************************************
 *  Method: Histogram2d::operator()
 *  Params: int x, int y
 * Returns: float &
 * Effects: 
 ***********************************************************************/
float &
Histogram2d::operator()(int x, int y)
{
	return mHistogram(x, y);
}


/***********************************************************************
 *  Method: Histogram2d::sampleInsertBinX
 *  Params: float x
 * Returns: int
 * Effects: 
 ***********************************************************************/
int
Histogram2d::sampleInsertBinX(float x) const
{
	if (getMinX() > x || getMaxX() < x)
		return -1;
	int indexX;
	indexX = (int)floor((float)mBinCountX * (x - getMinX()) / (getMaxX() - getMinX()));
	if (indexX == mBinCountX)
		indexX--;
	return indexX;
}


/***********************************************************************
 *  Method: Histogram2d::sampleInsertBinY
 *  Params: float y
 * Returns: int
 * Effects: 
 ***********************************************************************/
int
Histogram2d::sampleInsertBinY(float y) const
{
	if (getMinY() > y || getMaxY() < y)
		return -1;
	int indexY;
	indexY = (int)floor((float)mBinCountY * (y - getMinY()) / (getMaxY() - getMinY()));
	if (indexY == mBinCountY)
		indexY--;
	return indexY;
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
	int indexX = sampleInsertBinX(x);
	//if (indexX == -1)
	//	return false;
	int indexY = sampleInsertBinY(y);
	//if (indexY == -1)
	//	return false;
	if (indexX < 0 || indexX >= getBinCountX())
	{
		indexX = sampleInsertBinX(x);
		return false;
	}
	if (indexY < 0 || indexY >= getBinCountY())
	{
		indexY = sampleInsertBinY(y);
		return false;
	}


	mHistogram(indexX, indexY) += weight;
	return true;
}
