#pragma once
#include <map>
#include <string>
#include <functional>
#include <vector>
#include "Node.h"
#include "Segment.h"
#include "ONBasis.h"
#include "Axis.h"

class Axis;
class PFAxis; 
class LongAxis;
class SideAxis;
class Node;
class NodeSpec;
class Segment;

class Forest
{
public:
	Forest();
	Forest(const Forest& f);
	Forest(std::string filename, bool json);
	~Forest();

	void addNode(int ID, NodeSpec ns);

	Node* getNode(int nodeID);
	void deleteNode(int nodeID);
	void bypassNode(int nodeID);
	const Node* getNode(int nodeID) const;
	Segment* getSegment(int segmentID);
	const Segment* getSegment(int segmentID) const;
	std::pair<Node*, Segment*> getNodeAndSegment(int NSID);
	const std::pair<Node*, Segment*> getNodeAndSegment(int NSID) const;
	template <class T>
	void getRoots(T& NodeContainer) const;
	void treeWalk(int nodeID, std::function < void(int)> f);
	void treeWalk(int nodeID, std::function < void(int)> f) const;

	std::map<int, Node*>::const_iterator graphBegin() const;
	std::map<int, Node*>::const_iterator graphEnd() const;

	template <class T>
	void getTrees(T treeContainer) const;

	void reduce();
	Forest* generateForest(std::function<bool(const Node*, float&)> angleGenerator, vecN pf) const;
	void rebase(const ONBasis& B);
	void rescale(const vec& scale);
	void renumber();
	bool validate();
	void write(std::string filename);
	void samplePoints(
		std::vector<Point> &sample,
		int n,
		std::vector<int>* sampleSegmentIDs = nullptr,
		std::vector<float>* sampleSegmentRatios = nullptr) const;
	const std::map<int, Node*> getGraph() const { return mGraph; }
	float getWidth(vecN pf) const;
private:

	void initAxes();
	//std::map<int,std::pair<Node*,Segment*>> mGraph;
	std::map<int, Node*> mGraph;
	std::vector<std::string> header;
	bool error;
	PFAxis* mPF;
	LongAxis* mLong;
	SideAxis* mSide;
	
};

