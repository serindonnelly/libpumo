#pragma once
#include <vector>
#include <string>
#include <picojson.h>
#include "Forest.h"
#include "Segment.h"
#include "vec.h"

class Forest;
class Node;

class NodeSpec
{
public:
	NodeSpec(int type, Point xyz, float radius, int parentID);
	NodeSpec(const Node& n);
	~NodeSpec();
	int getParentID() const { return mParentID; }
	virtual void setParentID(int parentID) { mParentID = parentID;  }
	Point getPosition() const { return mPosition; }
	void setPosition(Point p) { mPosition = p; }
	float getRadius() const { return mRadius; }
	void setRadius(float r) { mRadius = r; }
	int getType() const { return mType; }
	bool isRoot() const { return mParentID == -1; }
protected:
	int mType;
	float mRadius;
	Point mPosition;
	int mParentID;
};

NodeSpec readNode(int& ID, const std::string& line);
NodeSpec readJSONNode(int& ID, const picojson::value& v);

class Node : public NodeSpec
{
public:
	Node(int ID, NodeSpec n, Forest* f);
	~Node();
	int getID() const { return mID; }
	Node* getParent() const;
	Segment* getSegment() const;
	Forest* getForest() const;
	void setParentID(int parentID);
	std::vector<int>::const_iterator beginChildren() const;
	std::vector<int>::const_iterator endChildren() const;
	std::vector<int>::iterator beginChildren();
	std::vector<int>::iterator endChildren();
	int numChildren() const;
	void removeChild(int nodeID);
	bool addChild(int nodeID);
	bool needsSegment() const;
	void createSegment();
	void destroySegment();
	bool hasChild(int nodeID) const;
private:
	int mID;
	Forest* mForest;
	std::vector<int> mChildren;
	Segment* mSegment;

};

