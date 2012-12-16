/*
 * nstree.cpp
 *
 *  Created on: Dec 12, 2012
 *      Author: chjd
 */

#include <iostream>
using std::cout;
using std::endl;
using std::cin;

#include "ngraph.h"
#include "nstree.h"

NSTNode::NSTNode(int e, NGraph* g) :
	eindex(e), graph(g), pl(nullptr), pr(nullptr),mark(false)
{
}

NSTree::NSTree(NGraph* g) :
	origin(g), root(nullptr)
{
	InitZeroOne();
}

NSTree::~NSTree()
{
	//ReleaseAllGraph();
	//ReleaseAllNode();
}

void NSTree::InitZeroOne()
{
	pZeroNSTNode = new NSTNode(0,nullptr);
	pOneNSTNode = new NSTNode(1,nullptr);
}

void NSTree::PrintAllPath()
{
	list<NSTNode*> paths;
	cout << "--- all paths begin..." << endl;
	CollectTermR(root,paths);
	cout << "- all paths done." << endl;
}

void NSTree::PrintTerm(list<NSTNode*>& paths)
{
	for (auto it = paths.begin(), et = paths.end(); it != et; it++)
	{
		cout << (*it)->eindex << "\t";
	}
	cout << endl;
}

void NSTree::CollectTermR(NSTNode* cn,list<NSTNode*>& paths)
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

void NSTree::ReleaseAllGraph()
{
	for (auto it = graphs.begin(), et = graphs.end(); it != et; it++)
	{
		delete (*it);
		(*it) = nullptr;
	}
	graphs.clear();
}

void NSTree::ReleaseAllNode()
{
	for (auto it = nodes.begin(), et = nodes.end(); it != et; it++)
	{
		delete (*it);
		(*it) = nullptr;
	}
	nodes.clear();
}

void NSTree::FindMinDegV(NGraph* graph, NGraph::ngv_it& minv)
{
	auto it = graph->vertexs.begin(), et = graph->vertexs.end();
	minv = it++;
	for (; it != et; it++)
	{
		if ((it->degree) < minv->degree)
		{
			minv = it;
		}
	}
}

NSTNode* NSTree::HOpen(NGraph* graph)
{
	/*
	 * if graph in sharedNGraph,
	 * 		return sharedNGraph[graph]
	 * to built horizontal,
	 *		find the minimal degree node,
	 *		//open edges on this node,
	 *		//	the first node, <= the first edge & the graph
	 *		//	other nodes, <= edge & the open-previous-edge graph
	 *
	 *		for edge in this node,
	 *			if edge & graph pair in sharedNodeMap,
	 *				share the left, and do not push into queue,
	 *			else,
	 *				create NSTNode, push into queue and sharedNodeMap
	 *				hash graph and node,
	 *				hash edge & graph pair and node
	 *			graph open edge
	 *	return the first node, to be the left
	 *
	 */
	NGraph::ngv_it minv;
	FindMinDegV(graph, minv);
	auto it = minv->edges.begin();

	int ce = *it;
	NGraph* cg = graph;
	NSTNode* first = new NSTNode(ce, cg);
	NSTNode* cn = first;
	layer.push(cn);

	graphs.push_back(cg);
	nodes.push_back(cn);
/*
	cout << "create node:" << endl;
	cout << cn->eindex << endl;
	cn->graph->Print();
*/

	it++;
	//for (auto et = minv->edges.end(); it != et; it++)
	int ecnt = 1;
	int etotal = minv->edges.size();
	while(ecnt<etotal)
	{
//		cout << ecnt << "\t" << etotal-1 << endl;
//		cout << "in while " << ecnt << endl;
		cg = new NGraph(*cg); // new NGraph
//		cout << "to open edge:" << ce << endl;
		cg->Open(ce);
		ce = *it;
		cn->pr = new NSTNode(ce, cg); // new NSTnode
		cn = cn->pr;
		layer.push(cn);

		it++;
		ecnt++;
		// save graph & node in list, for release memory
		graphs.push_back(cg);
		nodes.push_back(cn);
/*
		cout << "create node:" << endl;
		cout << cn->eindex << endl;
		cn->graph->Print();
*/
	}
	cn->pr = pZeroNSTNode;

	return first;
}

void NSTree::VShort(NSTNode* node)
{
	/*
	 * if node.graph short node.edge => 1
	 * 		node->pl = 1
	 * 	else
	 * 		node->pl = HOpen(node.graph)
	 */
	NGraph* cg = new NGraph(*(node->graph));
//	cout << "to short edge:" << node->eindex << endl;
	if (cg->Short(node->eindex))
	{
		node->pl = pOneNSTNode;
		delete cg;
	}
	else
	{
		node->pl = HOpen(cg);
	}
}

void NSTree::BFSBuild()
{
	cout << "---BFS build begin..." << endl;
	root = HOpen(origin);
	while (!layer.empty())
	{
		NSTNode* node = layer.front();
		VShort( node);
		layer.pop();
	}
	cout << "-BFS build done." << endl;
}
