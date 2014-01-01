#pragma once
#include "Analysis.h"
class ListAnalysis :
	public Analysis
{

public:
	ListAnalysis();
	~ListAnalysis();
	std::vector<std::string> contents;
private:
	virtual bool serialise(picojson::value& v) const;
	virtual bool deserialise(const picojson::value& v);
	virtual void updateImpl() {};
};

