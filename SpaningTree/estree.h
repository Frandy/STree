/*
 * estree.h
 *
 *  Created on: Dec 14, 2012
 *      Author: chjd
 */

#ifndef ESTREE_H_
#define ESTREE_H_

#include <list>
using std::list;
#include <queue>
using std::queue;
#include <stack>
using std::stack;
#include <unordered_map>
using std::unordered_map;
#include <map>
using std::map;

#include <string>
using std::u32string;

#include "egraph.h"
class EGraph;

class ESTNode
{
public:
	int eindex;
	EGraph* graph;

	ESTNode* pl;
	ESTNode* pr;

	bool visit;
	bool mark;

	int tid;	// triple index, for reduce
	ESTNode* pshare;	// shared triple,

public:
	ESTNode()=default;
	ESTNode(int e, EGraph* g);
	size_t Hash() const
	{
/*
		size_t t = graph->Hash();
		return (eindex*eindex + t*t);
*/
/* since each node with one graph, instead of graph-pair,
 * address of graph are the same <==> graph are the same, when graph sharing are used
 */
		return (eindex<<24) | ((size_t)graph & 0x00FFFFFF);
	}
	friend bool operator == (const ESTNode& a,const ESTNode& b)
	{
		EGEqual gequal;
		return (a.eindex==b.eindex) && (gequal(a.graph,b.graph));
	}
};

class ENodeHash
{
public:
	size_t operator () (const ESTNode* cn) const
	{
		return cn->Hash();
	}
};

class ENodeEqual
{
public:
	bool operator () (const ESTNode* a,const ESTNode* b) const
	{
		return (a==b)||((*a)==(*b));
	}
};

class ETripleHash
{
public:
	std::hash<u32string> hash_fn;
public:
	bool operator () (const ESTNode* cn) const
	{
	/*
		int symbId = cn->eindex;
		int leftId = cn->pl->tid;
		int rightId = cn->pr->tid;
		int al = (int)cn->pl;
		int ar = (int)cn->pr;
		size_t value = 0;
	//	value = (symbId*symbId)+(leftId*leftId<<8)+(rightId*rightId<<16);
		value = (symbId^leftId)<<8;
		value ^= (leftId^rightId);
		value ^= (al << 8) ^ (ar<<2);
	//	value = value&0x7FFFFFFF;
	//	cout << "triple hash value: " << value << endl;
		return value;
	*/

		u32string str(3,char32_t(0));
		str[0] = (char32_t(cn->eindex));
//		str[1] = (char32_t(cn->pl->eindex));
//		str[2] = (char32_t(cn->pr->eindex));
		str[1] = (char32_t(cn->pl->tid));
		str[2] = (char32_t(cn->pr->tid));
//		str[5] = (char32_t(int(cn->pl)));
//		str[6] = (char32_t(int(cn->pr)));
		return hash_fn(str);
	}
};

class ETripleEqual
{
public:
	bool operator () (const ESTNode* a,const ESTNode* b) const
	{
		return (a->eindex==b->eindex &&
				a->pl->tid==b->pl->tid &&
				a->pr->tid==b->pr->tid);
	}
};

class ETripleLessThan
{
public:
	bool operator () (const ESTNode* a,const ESTNode* b) const
	{
		typedef long long unsigned int UINT64;
		typedef unsigned int UINT32;
		UINT64 ta,tb;
		ta = ((UINT64(a->eindex)) << 54) \
				| ((UINT64((UINT32)(a->pl->tid)&0x08FFFFFF)) << 27) \
				| (UINT64((UINT32)(a->pl->tid)&0x08FFFFFF));
		tb = ((UINT64(b->eindex)) << 54) \
				| ((UINT64((UINT32)(b->pl->tid)&0x08FFFFFF)) << 27) \
				| (UINT64((UINT32)(b->pl->tid)&0x08FFFFFF));
		return ta<tb;
	}
};

class ESTree
{
public:
	EGraph* origin; // the original graph
	ESTNode* root; // root node
	list<ESTNode*> nodes; // all nodes
private:
	ESTNode* pZeroESTNode; // leaf node, zero & one
	ESTNode* pOneESTNode;
	list<ESTNode*> layer; // working layer

	// for sharing
	unordered_map<EGraph*,EGraph*,EGHash,EGEqual> sharedGraphMap;
	unordered_map<ESTNode*,ESTNode*,ENodeHash,ENodeEqual> sharedNodeMap;
//	typedef unordered_map<ESTNode*,ESTNode*,ETripleHash,ETripleEqual> SharedTripleMapT;
	typedef map<ESTNode*,ESTNode*,ETripleLessThan> SharedTripleMapT;
	SharedTripleMapT sharedTripleMap;

public:
	// construct
	ESTree()=default;
	ESTree(EGraph* g);
	~ESTree();
	// build & zero suppress & reduce
	void Build();

	// print
	void PrintAllPath();

private:
	void PrintTerm(list<ESTNode*>& paths);
	void CollectTermR(ESTNode* cn, list<ESTNode*>& paths,int& cnt);

	void AddNewNode(int eindex,ESTNode*& node,EGraph*& gl,int& cnt);

	void BFSBuild();
	void DFSBuild();
	void ZSuppress();
	void Reduce();
	void ZSuppressNodeR(ESTNode* cn,bool visit);
	void ReduceNodeR(ESTNode* cn,bool visit,int& tripleCnt);
	int ReleaseNodeGC();

	void InitZeroOne();
	// release memory of graph & node
	void ReleaseAllGraph();
	void ReleaseAllNode();
};

#endif /* ESTREE_H_ */
