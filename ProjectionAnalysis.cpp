#include "ProjectionAnalysis.h"
#include "ForestAnalysis.h"
#include "AxesAnalysis.h"


ProjectionAnalysis::ProjectionAnalysis()
{
}


ProjectionAnalysis::~ProjectionAnalysis()
{
}
/***********************************************************************
 *  Method: ProjectionAnalysis::serialise
 *  Params: picojson::value &v
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
ProjectionAnalysis::serialise(picojson::value &v) const
{
	picojson::object vo;
	picojson::array vlines;
	picojson::array vpoints;
	for (const auto& p : mPoints)
	{
		picojson::object vp;
		vp["x"] = picojson::value(p.x);
		vp["y"] = picojson::value(p.y);
		vpoints.push_back(picojson::value(vp));
	}
	for (const auto& l : mLines)
	{
		picojson::object vl;
		picojson::object start, end;
		start["x"] = picojson::value(l.first.x);
		start["y"] = picojson::value(l.first.y);
		end["x"] = picojson::value(l.second.x);
		end["y"] = picojson::value(l.second.y);
		vl["start"] = picojson::value(start);
		vl["end"] = picojson::value(end);
		vlines.push_back(picojson::value(vl));
	}
	vo["points"] = picojson::value(vpoints);
	vo["lines"] = picojson::value(vlines);
	vo["x_label"] = picojson::value(xLabel());
	vo["y_label"] = picojson::value(yLabel());
	v = picojson::value(vo);
	return true;
}


/***********************************************************************
 *  Method: ProjectionAnalysis::deserialise
 *  Params: const picojson::value &v
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
ProjectionAnalysis::deserialise(const picojson::value &v)
{
	return false;
}


/***********************************************************************
 *  Method: ProjectionAnalysis::updateImpl
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
ProjectionAnalysis::updateImpl()
{
	const Forest* f = ((ForestAnalysis*)inputs[0])->getForest();
	const vecN pf = ((AxesAnalysis*)inputs[1])->getPF();
	const vecN ln = ((AxesAnalysis*)inputs[1])->getLong();
	const vecN side = ((AxesAnalysis*)inputs[1])->getSide();
	for (const auto& idNode : f->getGraph())
	{
		Point n = idNode.second->getPosition();
		vec2 end = project(n, pf, ln, side);
		if (idNode.second->isRoot())
			mPoints.push_back(end);
		if (!idNode.second->isRoot())
		{
			Point p = idNode.second->getParent()->getPosition();
			vec2 start = project(p, pf, ln, side);
			mLines.push_back({ start, end });
		}
	}
}


