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

#include "egraph.h"
class EGraph;

class ESTNode
{
public:
	int eindex;
	EGraph* graph;

	ESTNode* pl;
	ESTNode* pr;

	bool mark;

public:
	ESTNode()=default;
	ESTNode(int e, EGraph* g);
};

class ESTree
{
public:
	EGraph* origin; // the original graph
	ESTNode* root; // root node
	list<ESTNode*> nodes; // all nodes
	list<EGraph*> graphs; // all graphs
private:
	ESTNode* pZeroESTNode; // leaf node, zero & one
	ESTNode* pOneESTNode;
	queue<ESTNode*> layer; // working layer

public:
	// construct
	ESTree()=default;
	ESTree(EGraph* g);
	~ESTree();
	// build
	void BFSBuild();

	void Reduce();
	void ZSupress();

	// print
	void PrintAllPath();
	void PrintTerm(list<ESTNode*>& paths);
	void CollectTermR(ESTNode* cn, list<ESTNode*>& paths);

	void InitZeroOne();
	// release memory of graph & node
	void ReleaseAllGraph();
	void ReleaseAllNode();
};

#endif /* ESTREE_H_ */
