#pragma once
#include "ForestAnalysis.h"
class ForestLoadAnalysis :
	public ForestAnalysis
{
public:
	ForestLoadAnalysis();
	~ForestLoadAnalysis();
private:
	virtual void updateImpl();
};

