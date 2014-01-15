#pragma once
#include <istream>
#include <picojson.h>
#include <string>
#include <sstream>
#include <TCL/unique_tree.h>

struct v3 { float x; float y; float z; };

class nnode_base
{
public: int id;
	nnode_base()
		: id(-1) {}
	nnode_base(int id_)
		: id(id_) {}
	virtual ~nnode_base(){}
	bool exists() const { return id != -1; }
	virtual nnode_base* clone() const { return new nnode_base(*this); }
	virtual void update_vec(v3 parent_vec) {}
	virtual v3 getPos() const { return{ FLT_MIN, FLT_MIN, FLT_MIN, }; }
};
static bool operator<(const nnode_base& lhs, const nnode_base& rhs) { return lhs.id < rhs.id; }

class nnode : public nnode_base
{
public:
	nnode(int id_, int type_, v3 pos_, float r_, int parent_)
		: nnode_base(id_), mPos(pos_), r(r_), type(type_), parent(parent_){}
	nnode(std::string line)
		: nnode_base()
	{
		std::stringstream ss(line);
		ss >> id >> type >> mPos.x >> mPos.y >> mPos.z >> r >> parent;
	}
	float r; int type; int parent;
	bool has_parent() const { return parent != -1; }
	const v3 seg() const { if (exists() && has_parent()) return mSeg; return{ 0.f, 0.f, 0.f }; }
	virtual nnode_base* clone() const { return new nnode(*this); }
	virtual void update_vec(v3 parent_vec) { mSeg = { parent_vec.x - mPos.x, parent_vec.y - mPos.y, parent_vec.z - mPos.z }; }
	virtual v3 getPos() const { return mPos; }
private:
	v3 mPos;
	v3 mSeg;
};

static nnode_base* clone_f(const nnode_base& n)
{
	return n.clone();
}

class TCLForest
{
public:
	TCLForest();
	~TCLForest();
	bool insert(nnode n);
	bool move(nnode n, v3 newPos);
	void readSWC(std::istream &is);
private:
	tcl::unique_tree < nnode_base > mForest;
	
};

