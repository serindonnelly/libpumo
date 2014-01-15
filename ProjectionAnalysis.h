#pragma once
#include <string>
#include "Analysis.h"
#include "vecN.h"
class ProjectionAnalysis :
	public Analysis
{
public:
	struct vec2{ float x; float y; vec2(float x_=0.f, float y_=0.f) : x(x_), y(y_){} };
	ProjectionAnalysis();
	~ProjectionAnalysis();
	virtual bool serialise(picojson::value& v) const;
	virtual bool deserialise(const picojson::value& v);
	virtual void updateImpl();
	virtual vec2 project(const vec& v, const vecN& pf, const vecN& ln, const vecN& sd) const = 0;
	virtual std::string xLabel() const = 0;
	virtual std::string yLabel() const = 0;
private:
	std::vector<std::pair<vec2, vec2>> mLines;
	std::vector<vec2> mPoints;
};

class LPProjectionAnalysis :
	public ProjectionAnalysis
{
	virtual vec2 project(const vec& v, const vecN& pf, const vecN& ln, const vecN& sd) const
	{
		return vec2(ln.dot(v), pf.dot(v));
	}
	virtual std::string xLabel() const { return "long"; };
	virtual std::string yLabel() const  { return "parallel fibre"; };

};

class LSProjectionAnalysis :
	public ProjectionAnalysis
{
	virtual vec2 project(const vec& v, const vecN& pf, const vecN& ln, const vecN& sd) const
	{
		return vec2(ln.dot(v), sd.dot(v));
	}
	virtual std::string xLabel() const { return "long"; };
	virtual std::string yLabel() const  { return "side"; };
};

class SPProjectionAnalysis :
	public ProjectionAnalysis
{
	virtual vec2 project(const vec& v, const vecN& pf, const vecN& ln, const vecN& sd) const
	{
		return vec2(sd.dot(v), pf.dot(v));
	}
	virtual std::string xLabel() const { return "side"; };
	virtual std::string yLabel() const  { return "parallel fibre"; };
};

