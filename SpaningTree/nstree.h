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

#include "ngraph.h"
class NGraph;

class NSTNode
{
public:
	int eindex;
	NGraph* graph;

	NSTNode* pl;
	NSTNode* pr;

	bool mark;

public:
	NSTNode()=default;
	NSTNode(int e,NGraph* g);
};

class NSTree
{
public:
	NGraph* origin;									// the original graph
	NSTNode* root;									// root node
	list<NSTNode*> nodes;							// all nodes
	list<NGraph*> graphs;							// all graphs
private:
	NSTNode* pZeroNSTNode;							// leaf node, zero & one
	NSTNode* pOneNSTNode;
	queue<NSTNode*> layer;							// working layer

//	unordered_map<NGraph*,NSTNode*> sharedNGraph;
//	unordered_map<NSTNode*,NSTNode*> sharedNodeMap;

public:
	// construct
	NSTree()=default;
	NSTree(NGraph* g);
	~NSTree();
	// build
	void BFSBuild();

	void Reduce();
	void ZSupress();

	// print
	void PrintAllPath();
	void PrintTerm(list<NSTNode*>& paths);
	void CollectTermR(NSTNode* cn,list<NSTNode*>& paths);

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

};

#endif /* NSTREE_H_ */
