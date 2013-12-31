#pragma once
#include <vector>
#include <Eigen/Core>


class Histogram2d
{
public:
	Histogram2d();
	~Histogram2d();
	float operator()(int x, int y) const;
	bool insertSample(float x, float y, float weight = 1.f);
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
private:
	void zero();
	Eigen::MatrixXf mHistogram;
	int binCountX;
	int binCountY;

	std::vector<float> binBoundariesX;
	std::vector<float> binBoundariesY;

	std::vector<float> binWeights;

};

