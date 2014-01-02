#pragma once
#include <vector>
#include <Eigen/Core>


class Histogram2d
{
public:
	Histogram2d();
	~Histogram2d();
	float operator()(int x, int y) const;
	float& operator()(int x, int y);
	//void setEntry(int x, int y, float v);
	bool insertSample(float x, float y, float weight = 1.f);
	int sampleInsertBinX(float x) const;
	int sampleInsertBinY(float y) const;
	void setBinCountX(int count);
	void setBinCountY(int count);
	void setRangeX(float minX, float maxX);
	void setRangeY(float minY, float maxY);
	void guessRangeX(const std::vector<float>& samples);
	void guessRangeY(const std::vector<float>& samples);
	float getMinX() const;
	float getMaxX() const;
	float getMinY() const;
	float getMaxY() const;
	int getBinCountX() const {
		return mBinCountX;
	};
	int getBinCountY() const {
		return mBinCountY;
	};
	void getBinBoundariesX(std::vector<float>& binBoundariesX) const;
	void getBinBoundariesY(std::vector<float>& binBoundariesY) const;

	float getBinBoundaryX(int i) const;
	float getBinBoundaryY(int i) const;

	void zero();

private:
	Eigen::MatrixXf mHistogram;
	int mBinCountX;
	int mBinCountY;

	float mMinX;
	float mMaxX;
	float mMinY;
	float mMaxY;

	//std::vector<float> binWeights;

};

