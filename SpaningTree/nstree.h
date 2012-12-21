/*
 * nstree.h
 *
 *  Created on: Dec 11, 2012
 *      Author: chjd
 */

#ifndef NSTREE_H_
#define NSTREE_H_

#include <list>
using std::list;
#include <queue>
using std::queue;
#include <stack>
using std::stack;
#include <unordered_map>
using std::unordered_map;

#include "ngraph.h"
class NGraph;

class NSTNode
{
public:
	int eindex;
	NGraph* graph;

	NSTNode* pl;
	NSTNode* pr;

	bool visit;
	bool mark;
	int tid;
	NSTNode* pshare;

public:
	NSTNode()=default;
	NSTNode(int e,NGraph* g);
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
	friend bool operator == (const NSTNode& a,const NSTNode& b)
	{
		NGEqual gequal;
		return (a.eindex==b.eindex) && (gequal(a.graph,b.graph));
	}
};

class NNodeHash
{
public:
	size_t operator () (const NSTNode* cn) const
	{
		return cn->Hash();
	}
};

class NNodeEqual
{
public:
	bool operator () (const NSTNode* a,const NSTNode* b) const
	{
		return (a==b)||((*a)==(*b));
	}
};

class NTripleHash
{
public:
	bool operator () (const NSTNode* cn) const
	{
		size_t t = cn->eindex << 10;
		t = t | (cn->pl->tid << 10);
		t = t | (cn->pr->tid << 10);
		return t;
	}
};

class NTripleEqual
{
public:
	bool operator () (const NSTNode* a,const NSTNode* b) const
	{
		return (a->eindex==b->eindex &&
				a->pl->tid==b->pl->tid &&
				a->pr->tid==b->pr->tid);
	}
};

class NSTree
{
public:
	NGraph* origin;									// the original graph
	NSTNode* root;									// root node
	list<NSTNode*> nodes;							// all nodes

private:
	NSTNode* pZeroNSTNode;							// leaf node, zero & one
	NSTNode* pOneNSTNode;
	queue<NSTNode*> layer;							// working layer

	// graph can be mapped with node
	unordered_map<NGraph*,NSTNode*,NGHash,NGEqual> sharedGraphNodeMap;
	unordered_map<NGraph*,NGraph*,NGHash,NGEqual> sharedGraphMap;
	unordered_map<NSTNode*,NSTNode*,NNodeHash,NNodeEqual> sharedNodeMap;
	unordered_map<NSTNode*,NSTNode*,NTripleHash,NTripleEqual> sharedTripleMap;

public:
	// construct
	NSTree()=default;
	NSTree(NGraph* g);
	~NSTree();
	// build
	void Build();
	void BFSBuild();
	// print
	void PrintAllPath();
	void PrintTerm(list<NSTNode*>& paths);
	void CollectTermR(NSTNode* cn,list<NSTNode*>& paths,int& cnt);

	void InitZeroOne();
	// release memory of graph & node
	void ReleaseAllGraph();
	void ReleaseAllNode();

private:
	// find the vertex with minimum degree
	void FindMinDegV(NGraph* graph,NGraph::ngv_it& minv);

	// open edges
	NSTNode* HOpen(NGraph* graph);

	// short edges
	void VShort(NSTNode* node);


	void Reduce();
	void ZSuppress();
	void ZSuppressNodeR(NSTNode* cn,bool visit);
	void ReduceNodeR(NSTNode* cn,bool visit,int& tripleCnt);
	int ReleaseNodeGC();
};

#endif /* NSTREE_H_ */
