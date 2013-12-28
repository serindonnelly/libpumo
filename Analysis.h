#pragma once
#include <string>
#include <vector>
#include <iomanip>

class Analysis;

class Analysis
{
public:
	Analysis();
	~Analysis();
	void addInput(Analysis* input);
	time_t getUpdateTime();
	void update();
	virtual void load() = 0;
	void setIdentity(std::string id, int order);

protected:
	std::string getFilename() { return mFilename; }
	std::vector<Analysis*> inputs;
	std::vector<Analysis*> allInputs;
private:
	time_t mUpdated;
	std::string mFilename;
	std::string mIdentity;
	int mOrder;
	virtual void updateImpl() = 0;
	virtual void save() = 0;
};

