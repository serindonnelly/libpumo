#pragma once
#include "ForestAnalysis.h"
class ForestLoadAnalysis :
	public ConcreteForestAnalysis
{
public:
	ForestLoadAnalysis();
	~ForestLoadAnalysis();
private:
	virtual void updateImpl();
};

