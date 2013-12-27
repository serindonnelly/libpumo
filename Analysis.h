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
	void setIdentity(std::string id);
protected:
	std::string getFilename() { return filename; }
private:
	std::vector<Analysis*> inputs;
	std::vector<Analysis*> allInputs;
	time_t updated;
	std::string filename;
	std::string identity;
	virtual void updateImpl() = 0;
	virtual void save() = 0;
};

