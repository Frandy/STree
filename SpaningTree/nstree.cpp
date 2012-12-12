/*
 * nstree.cpp
 *
 *  Created on: Dec 12, 2012
 *      Author: chjd
 */

#include "ngraph.h"
#include "nstree.h"

NSTNode::NSTNode(int e, NGraph* g) :
	eindex(e), graph(g), pl(nullptr), pr(nullptr),mark(false)
{
}

NSTree::NSTree(NGraph* g) :
	origin(g), root(nullptr)
{
}

NSTree::~NSTree()
{
	ReleaseAllGraph();
	ReleaseAllNode();
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

	it++;
	for (auto et = minv->edges.end(); it != et; it++)
	{
		cg = new NGraph(*cg); // new NGraph
		cg->Open(ce);
		ce = *it;
		cn->pr = new NSTNode(ce, cg); // new NSTnode
		cn = cn->pr;
		layer.push(cn);

		// save graph & node in list, for release memory
		graphs.push_back(cg);
		nodes.push_back(cn);
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
	root = HOpen(origin);
	while (!layer.empty())
	{
		NSTNode* node = layer.front();
		VShort( node);
		layer.pop();
	}
}
