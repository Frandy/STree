/*
 * estree.cpp
 *
 *  Created on: Dec 14, 2012
 *      Author: chjd
 */

#include <iostream>
using std::cout;
using std::endl;
using std::cin;

#include "egraph.h"
#include "estree.h"

ESTNode::ESTNode(int e, EGraph* g) :
	eindex(e), graph(g), pl(nullptr), pr(nullptr),mark(false)
{
}

ESTree::ESTree(EGraph* g) :
	origin(g), root(nullptr)
{
	InitZeroOne();
}

ESTree::~ESTree()
{
	//ReleaseAllGraph();
	//ReleaseAllNode();
}

void ESTree::InitZeroOne()
{
	pZeroESTNode = new ESTNode(0,nullptr);
	pOneESTNode = new ESTNode(1,nullptr);
	nodes.push_back(pZeroESTNode);
	nodes.push_back(pOneESTNode);
}

void ESTree::PrintAllPath()
{
	list<ESTNode*> paths;
	cout << "--- all paths begin..." << endl;
	CollectTermR(root,paths);
	cout << "- all paths done." << endl;
}

void ESTree::PrintTerm(list<ESTNode*>& paths)
{
	for (auto it = paths.begin(), et = paths.end(); it != et; it++)
	{
		cout << (*it)->eindex << "\t";
	}
	cout << endl;
}

void ESTree::CollectTermR(ESTNode* cn,list<ESTNode*>& paths)
{
	if (cn->eindex == 1)
	{
		//cout << "--- cn = 1" << endl;
		PrintTerm(paths);
	}
	else if(cn->eindex==0)
	{
		//cout << "... cn = 0" << endl;
		return;
	}
	else
	{
		//cout << "push in cn:" << cn->eindex << endl;
		paths.push_back(cn);
		CollectTermR(cn->pl, paths);
		//cout << "pop cn:" << paths.back()->eindex << endl;
		paths.pop_back();
		CollectTermR(cn->pr, paths);
	}
}

void ESTree::ReleaseAllGraph()
{
	for (auto it = graphs.begin(), et = graphs.end(); it != et; it++)
	{
		delete (*it);
		(*it) = nullptr;
	}
	graphs.clear();
}

void ESTree::ReleaseAllNode()
{
	for (auto it = nodes.begin(), et = nodes.end(); it != et; it++)
	{
		delete (*it);
		(*it) = nullptr;
	}
	nodes.clear();
}


void ESTree::BFSBuild()
{
	cout << "---BFS build begin..." << endl;

	root = new ESTNode(origin->edges.front().index,origin);
	layer.push(root);
	while(!layer.empty())
	{
		ESTNode* cn = layer.front();

		// short eindex to get pl
		EGraph* gl = new EGraph(*(cn->graph));
		int okl = gl->Short(cn->eindex);
		if(okl==1)
			cn->pl = pOneESTNode;
		else if(okl==0)
			cn->pl = pZeroESTNode;
		else
		{
			cn->pl = new ESTNode(cn->eindex+1,gl);
			layer.push(cn->pl);
		}

		// open eindex to get pr
		EGraph* gr = new EGraph(*(cn->graph));
		int okr = gr->Open(cn->eindex);
		if(ok==0)
			cn->pr = pZeroESTNode;
		else
		{
			cn->pr = new ESTNode(cn->eindex+1,gr);
			layer.push(cn->pr);
		}
		layer.pop();
	}
	cout << "-BFS build done." << endl;
}
