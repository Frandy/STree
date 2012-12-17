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


	static int totalcnt;

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
	bool operator () (const ESTNode* cn) const
	{
		size_t t = cn->eindex << 10;
		t = t | (cn->pl->tid << 10);
		t = t | (cn->pr->tid << 10);
		return t;
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

class ESTree
{
public:
	EGraph* origin; // the original graph
	ESTNode* root; // root node
	list<ESTNode*> nodes; // all nodes
	list<EGraph*> graphs; // all graphs, since there are sharedGraphMap
private:
	ESTNode* pZeroESTNode; // leaf node, zero & one
	ESTNode* pOneESTNode;
	list<ESTNode*> layer; // working layer

	// for sharing
	unordered_map<EGraph*,EGraph*,EGHash,EGEqual> sharedGraphMap;
	unordered_map<ESTNode*,ESTNode*,ENodeHash,ENodeEqual> sharedNodeMap;
	unordered_map<ESTNode*,ESTNode*,ETripleHash,ETripleEqual> sharedTripleMap;

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
	void CollectTermR(ESTNode* cn, list<ESTNode*>& paths);

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
