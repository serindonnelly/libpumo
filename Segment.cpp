#include "Segment.h"


Segment::Segment(int nodeID, Forest* f) : mNodeID(nodeID), mForest(f), mVector(0,0,0)
{
	mParentID = mForest->getNode(nodeID)->getParentID();
	update();
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


///***********************************************************************
// *  Method: Segment::getVector
// *  Params: 
// * Returns: Displacement
// * Effects: 
// ***********************************************************************/
//Displacement
//Segment::getVector() const
//{
//	return getDistalNode()->getPosition() - getProximalNode()->getPosition();
//}
/***********************************************************************
*  Method: Segment::getCachedVector
*  Params:
* Returns: Displacement
* Effects:
***********************************************************************/
Displacement
Segment::getVector() const
{
	return mVector;
}
/***********************************************************************
 *  Method: Segment::getPoint
 *  Params: float fraction
 * Returns: Point
 * Effects: low fraction produces point nearer to parent
 ***********************************************************************/
Point
Segment::getPoint(float fraction) const
{
	return getProximalNode()->getPosition() + fraction*mVector;
}


/***********************************************************************
 *  Method: Segment::updateVector
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Segment::update()
{
	mVector = getDistalNode()->getPosition() -getProximalNode()->getPosition();
}


