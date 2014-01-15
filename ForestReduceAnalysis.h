#pragma once
#include "ForestAnalysis.h"
class ForestReduceAnalysis :
	public ConcreteForestAnalysis
{
public:
	ForestReduceAnalysis();
	~ForestReduceAnalysis();
	virtual void updateImpl();
};

