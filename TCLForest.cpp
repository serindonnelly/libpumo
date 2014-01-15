#include "TCLForest.h"


TCLForest::TCLForest()
{
	mForest.set_clone(&clone_f);
	nnode_base b;
	mForest.insert(b);
}


TCLForest::~TCLForest()
{
}
/***********************************************************************
 *  Method: TCLForest::insert
 *  Params: nnode n
 * Returns: bool
 * Effects: 
 ***********************************************************************/
bool
TCLForest::insert(nnode n)
{
	auto it = mForest.insert(n.parent, n);
	if (it == mForest.end())
		return false;
	if (!it.node()->is_root())
	{
		const auto& parent = it.node()->parent()->get();
		it->update_vec(parent->getPos());
	}
	return true;
}


void
TCLForest::readSWC(std::istream &is)
{
	std::string line;
	while (std::getline(is, line))
	{
		if (line.length() == 0) continue;
		if (line[0] == '#')
		{
			//header.push_back(line);
			continue;
		}
		nnode n(line);
		insert(n);
	}
}
