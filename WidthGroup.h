#pragma once
#include <picojson.h>
struct WidthGroup
{
	float widthIQ;
	float widthSD;
	float widthAbs;
	WidthGroup(){};
	WidthGroup(float IQ, float SD, float Abs) :
		widthIQ(IQ), widthSD(SD), widthAbs(Abs) {};
	WidthGroup(const picojson::value& v) :
		widthIQ(-1.f), widthSD(-1.f), widthAbs(-1.f)
	{
		//can't use jget, jat because they refer to this
		if (!v.is<picojson::object>()) return;

		if (!v.contains("IQ")) return;
		if (!v.contains("SD")) return;
		if (!v.contains("Abs")) return;

		if (!v.get("IQ").is<double>()) return;
		if (!v.get("SD").is<double>()) return;
		if (!v.get("Abs").is<double>()) return;

		widthIQ = (float)v.get("IQ").get<double>();
		widthSD = (float)v.get("SD").get<double>();
		widthAbs = (float)v.get("Abs").get<double>();
	};
	WidthGroup operator+(const WidthGroup& right) const
	{
		return WidthGroup(widthIQ + right.widthIQ,
			widthSD + right.widthSD,
			widthAbs + right.widthAbs);
	}
	template<typename T>
	WidthGroup operator/(const T& right) const
	{
		return WidthGroup(widthIQ / right,
			widthSD / right,
			widthAbs / right);
	}
	WidthGroup operator/(const WidthGroup& right) const
	{
		return WidthGroup(widthIQ / right.widthIQ,
			widthSD / right.widthSD,
			widthAbs / right.widthAbs);
	}
	WidthGroup operator*(const WidthGroup& right) const
	{
		return WidthGroup(widthIQ * right.widthIQ,
			widthSD * right.widthSD,
			widthAbs * right.widthAbs);
	}
	WidthGroup operator-(const WidthGroup& right) const
	{
		return *this + right / -1;
	}
	WidthGroup sqrtWG() const
	{
		return WidthGroup(sqrt(widthIQ), sqrt(widthSD), sqrt(widthAbs));
	}
	picojson::value serialise() const
	{
		picojson::object vo;
		vo["IQ"] = picojson::value(widthIQ);
		vo["SD"] = picojson::value(widthSD);
		vo["Abs"] = picojson::value(widthAbs);
		return picojson::value(vo);
	}
	bool isValid() const
	{
		return widthIQ >= 0.f &&
			widthSD >= 0.f &&
			widthAbs >= 0.f;
	}
};
