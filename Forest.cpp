#include <random>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <set>
#include <initializer_list>


#include "Forest.h"



Forest::Forest()
{

}

Forest::~Forest()
{

}


/***********************************************************************
 *  Method: Forest::Forest
 *  Params: Forest &f
 * Effects: copies the input forest and all its contents
 ***********************************************************************/
Forest::Forest(const Forest &f) : error(false)
{
	// TODO copy header
	for (auto ir = f.graphBegin(); ir != f.graphEnd(); ++ir)
	{
		NodeSpec ns(*(*ir).second);
		addNode((*ir).first, ns);
	}
	//f.treeWalk()
}


/***********************************************************************
 *  Method: Forest::Forest
 *  Params: std::string filename
 * Effects: 
 ***********************************************************************/
Forest::Forest(std::string filename) : error(false)
{
	std::ifstream file(filename);

	std::string line;
	while (std::getline(file, line))
	{
		if (line.length() == 0) continue;
		if (line[0] == '#')
		{
			header.push_back(line);
			continue;
		}
		int ID;
		NodeSpec ns = readNode(ID, line);
		addNode(ID, ns);
	}
}


/***********************************************************************
 *  Method: Forest::reduce
 *  Params: 
 * Returns: Forest *
 * Effects: 
 ***********************************************************************/
void
Forest::reduce()
{
	std::vector<int> roots;
	getRoots(roots);
	std::set<int> intermediateNodes;
	for (auto it = roots.begin(); it != roots.end(); ++it)
	{
		treeWalk((*it), [&](int nodeID)
		{
			Node* n = getNode(nodeID);
			if (n->isRoot()) return;
			if (n->numChildren() == 1)
				intermediateNodes.insert(nodeID);
		}
		);
	}
	for (auto it = intermediateNodes.begin(); it != intermediateNodes.end(); ++it)
	{
		Node* n = getNode(*it);
		Node* child = getNode(*n->beginChildren());
		Node* parent = n->getParent();
		child->setParentID(n->getParentID());
		for (auto ic = parent->beginChildren(); ic != parent->endChildren(); ++ic)
		{
			if (*ic == *it)
				*ic = child->getID();
		}
		delete n;
		mGraph.erase(*it);
	}
}


void Forest::treeWalk(int nodeID, std::function < void(int)> f)
{
	Node* currentNode = getNode(nodeID);
	f(nodeID);
	for (auto ic = currentNode->beginChildren(); ic != currentNode->endChildren(); ++ic)
	{
		treeWalk(*ic,f);
	}
}


/***********************************************************************
 *  Method: Forest::getNode
 *  Params: int nodeID
 * Returns: Node *
 * Effects: 
 ***********************************************************************/
Node *
Forest::getNode(int nodeID)
{
	return mGraph.at(nodeID);
}


/***********************************************************************
 *  Method: Forest::getNode
 *  Params: int nodeID
 * Returns: const Node *
 * Effects: 
 ***********************************************************************/
const Node *
Forest::getNode(int nodeID) const
{
	return mGraph.at(nodeID);
}


/***********************************************************************
 *  Method: Forest::getSegment
 *  Params: int segmentID
 * Returns: Segment *
 * Effects: 
 ***********************************************************************/
Segment *
Forest::getSegment(int segmentID)
{
	return mGraph.at(segmentID)->getSegment();
}


/***********************************************************************
 *  Method: Forest::getSegment
 *  Params: int segmentID
 * Returns: const Segment *
 * Effects: 
 ***********************************************************************/
const Segment *
Forest::getSegment(int segmentID) const
{
	return mGraph.at(segmentID)->getSegment();
}


/***********************************************************************
 *  Method: Forest::getNodeAndSegment
 *  Params: int NSID
 * Returns: std::pair<Node *, Segment *>
 * Effects: 
 ***********************************************************************/
std::pair<Node *, Segment *>
Forest::getNodeAndSegment(int NSID)
{
	return std::make_pair(mGraph.at(NSID), mGraph.at(NSID)->getSegment());
}


/***********************************************************************
 *  Method: Forest::getNodeAndSegment
 *  Params: int NSID
 * Returns: const std::pair<Node *, Segment *>
 * Effects: 
 ***********************************************************************/
const std::pair<Node *, Segment *>
Forest::getNodeAndSegment(int NSID) const
{
	return std::make_pair(mGraph.at(NSID), mGraph.at(NSID)->getSegment());
}


/***********************************************************************
 *  Method: Forest::generateForest
 *  Params: std::function<float(Node*)> angleGenerator
 * Returns: Forest*
 * Effects: 
 ***********************************************************************/
Forest*
Forest::generateForest(std::function<bool(Node*,float& angle)> angleGenerator, vecN pf)
{
	// TODO copy header
	Forest* f = new Forest();
	std::vector<int> roots;
	getRoots(roots);
	for (auto ir = roots.begin(); ir != roots.end(); ++ir)
	{
		treeWalk(*ir, [&](int nodeID)
		{
			Node* n = getNode(nodeID);
			NodeSpec ns(*n);

			f->addNode(nodeID, ns);
			Node* newNode = f->getNode(nodeID);
			if (!newNode->isRoot())
			{
				Displacement seg = n->getSegment()->getVector();
				float angle;
				if (angleGenerator(n, angle))
				{
					vec horizontal = pf.projectOrth(seg);
					vecN horizontalUnit(horizontal);
					seg = cos(angle)*horizontalUnit + sin(angle)*pf;
				}
				newNode->setPosition(newNode->getParent()->getPosition() + seg);
			}
		}
		);
	}
	return f;
}

std::map<int, Node*>::const_iterator Forest::graphBegin() const
{
	return mGraph.begin();
}

std::map<int, Node*>::const_iterator Forest::graphEnd() const
{
	return mGraph.end();
}

/***********************************************************************
 *  Method: Forest::rebase
 *  Params: const ONBasis &B
 * Returns: Forest *
 * Effects: 
 ***********************************************************************/
void
Forest::rebase(const ONBasis &B)
{
	for (auto it = graphBegin(); it != graphEnd(); ++it)
	{
		Node* n = (*it).second;
		n->setPosition(B.rebase(n->getPosition()));
	}
}


/***********************************************************************
 *  Method: Forest::addNode
 *  Params: NodeSpec n
 * Returns: int
 * Effects: 
 ***********************************************************************/
void
Forest::addNode(int ID, NodeSpec ns)
{
	if (mGraph.find(ID) != mGraph.end())
	{
		error = true;
		return;
	}
	Node* n = new Node(ID, ns, this);
	mGraph[ID] = n;
	n->createSegment();
}


/***********************************************************************
 *  Method: Forest::rescale
 *  Params: const vec &scale
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Forest::rescale(const vec &scale)
{
	float vscale = cbrt(scale.x*scale.y*scale.z);
	for (auto it = graphBegin(); it != graphEnd(); ++it)
	{
		Node* n = (*it).second;
		Point p = n->getPosition();
		Point pp(p.x*scale.x, p.x*scale.y, p.z*scale.z);
		n->setPosition(pp);
		n->setRadius(vscale*n->getRadius());
	}
}


/***********************************************************************
 *  Method: Forest::renumber
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Forest::renumber()
{
}

template <class T>
void Forest::getRoots(T& NodeContainer) const
{
	for (auto it = mGraph.begin(); it != mGraph.end(); ++it)
	{
		if ((*it).second->isRoot())
			NodeContainer.push_back((*it).first);
	}
}
/***********************************************************************
 *  Method: Forest::deleteNode
 *  Params: int nodeID
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Forest::deleteNode(int nodeID)
{
	Node* n = getNode(nodeID);

	for (auto ic = n->beginChildren(); ic != n->endChildren(); ++ic)
	{
		getNode(*ic)->setParentID(-1);
		
	}
	if (!n->isRoot())
	{
		n->getParent()->removeChild(nodeID);
		delete getSegment(nodeID);
	}
	delete n;
}


/***********************************************************************
 *  Method: Forest::bypassNode
 *  Params: int nodeID
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Forest::bypassNode(int nodeID)
{
	Node* n = getNode(nodeID);
	if (!n->isRoot())
	{
		std::vector<int> children;
		for (auto ic = n->beginChildren(); ic != n->endChildren(); ++ic)
		{
			children.push_back(*ic);
			getNode(*ic)->setParentID(n->getParentID());
			n->getParent()->addChild(*ic);
		}
		for (auto ic = children.begin(); ic != children.end(); ++ic)
		{
			n->removeChild(*ic);
		}
	}
	deleteNode(nodeID);
}


/***********************************************************************
 *  Method: Forest::validate
 *  Params: 
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
Forest::validate()
{
	for (auto it = graphBegin(); it != graphEnd(); ++it)
	{
		Node* n = (*it).second;
		if (n->needsSegment())
			return false;
		if (!n->isRoot())
		{
			if (n->getParent() == nullptr)
				return false;
			if (!n->getParent()->hasChild(n->getID()))
				return false;
		}
		else
		{
			if (n->getSegment() != nullptr)
				return false;
		}
		for (auto ic = n->beginChildren(); ic != n->endChildren(); ++ic)
		{
			if (!n->hasChild(*ic))
				return false;
		}
	}
	return true;

}


/***********************************************************************
 *  Method: Forest::write
 *  Params: std::string filename
 * Returns: bool
 * Effects: 
 ***********************************************************************/
void
Forest::write(std::string filename)
{
	std::ofstream file(filename);
	for (auto ih = header.begin(); ih != header.end(); ++ih)
	{
		file << *ih;
	}
	for (auto in = graphBegin(); in != graphEnd(); ++in)
	{
		Node* n = (*in).second;
		file << n->getID() << ' ' << n->getType() << ' ' << n->getPosition().x << ' ' << n->getPosition().y << ' ' << n->getPosition().z << ' ' << n->getRadius() << ' ' << n->getParentID() << '\n';
	}
}


/***********************************************************************
 *  Method: Forest::samplePoints
 *  Params: std::vector<Point> &sample, int n
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
Forest::samplePoints(std::vector<Point> &sample, int n)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	int maxNode = (*mGraph.rbegin()).first;
	std::discrete_distribution<int> segmentSelector(maxNode, 0, maxNode, [&](int i)->float
	{
		auto it = mGraph.find(i);
		if (it == mGraph.end())
		{
			return 0.f;
		}
		else
		{
			Node* n = (*it).second;
			if (n->isRoot())
				return 0.f;
			else
				return n->getSegment()->getVector().norm();
		}
	}
	);
	std::uniform_real_distribution<float> fractions(0.f, 1.f);
	for (int i = 0; i < n; i++)
	{
		Segment* s = getSegment(segmentSelector(gen));
		sample.push_back(s->getPoint(fractions(gen)));
	}
}


