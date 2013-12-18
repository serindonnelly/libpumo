#pragma once
#include "vec.h"
#include "Node.h"
#include "Forest.h"
class Node;
class Forest;
class Segment
{
public:
	Segment(int nodeID, Forest* f);
	~Segment();
	Node* getProximalNode() const;
	Node* getDistalNode() const;
	Forest* getForest() const;
	Displacement getVector() const;
	Point getPoint(float fraction) const;
private:
	int mNodeID;
	int mParentID;
	Forest* mForest;
};

