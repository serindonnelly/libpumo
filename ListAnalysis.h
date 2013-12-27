#pragma once
#include "Analysis.h"
class ListAnalysis :
	public Analysis
{

public:
	ListAnalysis();
	~ListAnalysis();
	virtual void load() {};
	std::vector<std::string> contents;
private:
	virtual void save() {};
	virtual void updateImpl() {};
};

