#pragma once
#include "NullAnalysis.h"
class ListAnalysis :
	public NullAnalysis
{

public:
	ListAnalysis();
	~ListAnalysis();
	std::vector<std::string> contents;
private:
};

