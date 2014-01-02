#pragma once
#include <string>
#include <vector>
#include <iomanip>
#include <picojson.h>

class Analysis;

class Analysis
{
public:
	Analysis();
	~Analysis();
	void addInput(Analysis* input);
	time_t getUpdateTime();
	void update();
	virtual bool load();
	void setIdentity(std::string id, int order);
	void setFilename(std::string fn) {
		mFilename = fn;
	}
protected:
	std::string getFilename() { return mFilename; }
	std::vector<Analysis*> inputs;
	std::vector<Analysis*> allInputs;
private:
	//serialise should produce a json value which when fed through
	//deserialise would produce an equivalent Analysis instance
	virtual bool serialise(picojson::value& v) const = 0;
	//deserialise should leave no changes to the Analysis instance
	//if it returns false
	virtual bool deserialise(const picojson::value& v) = 0;
	time_t mUpdated;
	std::string mFilename;
	std::string mIdentity;
	int mOrder;
	virtual void updateImpl() = 0;
	virtual void save();
};

