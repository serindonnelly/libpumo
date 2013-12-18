#include "Node.h"
#include <sstream>
#include <algorithm>

NodeSpec::~NodeSpec()
{
}

Node::~Node()
{
	delete mSegment;
}


/***********************************************************************
 *  Method: Node::Node
 *  Params: int ID, Point xyz, float radius, int parentID, Forest* f
 * Effects: sets all members to given values
 ***********************************************************************/
NodeSpec::NodeSpec(int type, Point xyz, float radius, int parentID) :
mType(type), mPosition(xyz), mRadius(radius), mParentID(parentID)
{
}

/***********************************************************************
*  Method: Node::Node
*  Params: std::string line
* Effects:
***********************************************************************/
NodeSpec readNode(int& ID, std::string line)
{
	int type;
	float x, y, z;
	float radius;
	int parentID;
	std::stringstream ss(line);
	ss >> ID >> type >> x >> y >> z >> radius >> parentID;
	return NodeSpec(type, Point(x, y, z), radius, parentID);
}


Node::Node(int ID, NodeSpec n, Forest* f) : mID(ID), NodeSpec(n), mForest(f)
{

}

/***********************************************************************
 *  Method: Node::getParent
 *  Params: 
 * Returns: Node *
 * Effects: 
 ***********************************************************************/
Node *
Node::getParent() const
{
	return mForest->getNode(mParentID);
}


/***********************************************************************
 *  Method: Node::getSegment
 *  Params: 
 * Returns: Segment *
 * Effects: 
 ***********************************************************************/
Segment *
Node::getSegment() const
{
	return mSegment;
}


/***********************************************************************
 *  Method: Node::getForest
 *  Params: 
 * Returns: Forest *
 * Effects: 
 ***********************************************************************/
Forest *
Node::getForest() const
{
	return mForest;
}

std::vector<int>::const_iterator Node::beginChildren() const
{
	return mChildren.begin();
}

std::vector<int>::const_iterator Node::endChildren() const
{
	return mChildren.end();
}

std::vector<int>::iterator Node::beginChildren()
{
	return mChildren.begin();
}

std::vector<int>::iterator Node::endChildren()
{
	return mChildren.end();
}

/***********************************************************************
 *  Method: Node::numChildren
 *  Params: 
 * Returns: int
 * Effects: 
 ***********************************************************************/
int
Node::numChildren() const
{
	return mChildren.size();
}


/***********************************************************************
 *  Method: NodeSpec::NodeSpec
 *  Params: Node &n
 * Effects: 
 ***********************************************************************/
NodeSpec::NodeSpec(const Node &n) : mPosition(n.getPosition()), mType(n.getType()), mRadius(n.getRadius()), mParentID(n.getParentID())
{

}


/***********************************************************************
 *  Method: Node::removeChild
 *  Params: int NodeID
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Node::removeChild(int nodeID)
{
	mChildren.erase(std::remove(mChildren.begin(), mChildren.end(), nodeID));
}


/***********************************************************************
 *  Method: Node::addChild
 *  Params: int NodeID
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
Node::addChild(int nodeID)
{
	if (nodeID > mID)
	{
		mChildren.push_back(nodeID);
		return true;
	}
	else
		return false;
}


/***********************************************************************
 *  Method: Node::setParentID
 *  Params: int parentID
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Node::setParentID(int parentID)
{

	NodeSpec::setParentID(parentID);
	if (mParentID == -1)
	{
		if (mForest)
			delete getSegment();
	}
}


/***********************************************************************
 *  Method: Node::needsSegment
 *  Params: 
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
Node::needsSegment() const
{
	return mParentID != -1 && !mSegment && mForest != nullptr;
}


/***********************************************************************
 *  Method: Node::createSegment
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Node::createSegment()
{
	if (!needsSegment())
		return;
	mSegment = new Segment(mID, mForest);
	getParent()->addChild(mID);
}


/***********************************************************************
 *  Method: Node::destroySegment
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Node::destroySegment()
{
	if (mSegment)
		delete mSegment;
	mSegment = nullptr;
	if (!isRoot())
		getParent()->removeChild(mID);

}


/***********************************************************************
 *  Method: Node::isChild
 *  Params: int nodeID
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
Node::hasChild(int nodeID) const
{
	if (mChildren.size() == 0)
		return false;
	return std::find(mChildren.begin(), mChildren.end(), nodeID) != mChildren.end();
}


