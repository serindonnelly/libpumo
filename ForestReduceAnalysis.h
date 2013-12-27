#pragma once
#include "ForestAnalysis.h"
class ForestReduceAnalysis :
	public ForestAnalysis
{
public:
	ForestReduceAnalysis();
	~ForestReduceAnalysis();
	virtual void updateImpl();
};

