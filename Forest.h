#pragma once
#include <map>
#include <string>
#include <functional>
#include "Node.h"
#include "Segment.h"
#include "ONBasis.h"
#include <vector>

class Node;
class NodeSpec;
class Segment;

class Forest
{
public:
	Forest();
	Forest(const Forest& f);
	Forest(std::string filename);
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

	std::map<int, Node*>::const_iterator graphBegin() const;
	std::map<int, Node*>::const_iterator graphEnd() const;

	template <class T>
	void getTrees(T treeContainer) const;

	void reduce();
	Forest* generateForest(std::function<bool(Node*, float&)> angleGenerator, vecN pf);
	void rebase(const ONBasis& B);
	void rescale(const vec& scale);
	void renumber();
	bool validate();
	void write(std::string filename);
	void samplePoints(std::vector<Point>& sample, int n);
private:
	//std::map<int,std::pair<Node*,Segment*>> mGraph;
	std::map<int, Node*> mGraph;
	std::vector<std::string> header;
	bool error;
};

