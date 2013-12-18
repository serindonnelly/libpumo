#include "Segment.h"


Segment::Segment(int nodeID, Forest* f) : mNodeID(nodeID), mForest(f)
{
	mParentID = mForest->getNode(nodeID)->getParentID();
}


Segment::~Segment()
{
}
/***********************************************************************
 *  Method: Segment::getProximalNode
 *  Params: 
 * Returns: Node *
 * Effects: 
 ***********************************************************************/
Node *
Segment::getProximalNode() const
{
	return mForest->getNode(mParentID);
}


/***********************************************************************
 *  Method: Segment::getDistalNode
 *  Params: 
 * Returns: Node *
 * Effects: 
 ***********************************************************************/
Node *
Segment::getDistalNode() const
{
	return mForest->getNode(mNodeID);
}


/***********************************************************************
 *  Method: Segment::getForest
 *  Params: 
 * Returns: Forest *
 * Effects: 
 ***********************************************************************/
Forest *
Segment::getForest() const
{
	return mForest;
}


/***********************************************************************
 *  Method: Segment::getVector
 *  Params: 
 * Returns: Displacement
 * Effects: 
 ***********************************************************************/
Displacement
Segment::getVector() const
{
	return getDistalNode()->getPosition() - getProximalNode()->getPosition();
}
/***********************************************************************
 *  Method: Segment::getPoint
 *  Params: float fraction
 * Returns: Point
 * Effects: 
 ***********************************************************************/
Point
Segment::getPoint(float fraction) const
{
	return getProximalNode()->getPosition() + fraction*getVector();
}


