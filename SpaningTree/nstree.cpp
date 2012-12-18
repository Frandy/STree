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
#include <utility>
using std::make_pair;

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
	pZeroNSTNode->pl = pOneNSTNode;
	pZeroNSTNode->pr = pZeroNSTNode;
	pOneNSTNode->pl = pOneNSTNode;
	pOneNSTNode->pr = pZeroNSTNode;
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
	for (auto it = sharedGraphMap.begin(), et = sharedGraphMap.end(); it != et; it++)
	{
		delete (it->second);
		(it->second) = nullptr;
	}
	sharedGraphMap.clear();
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
	//	if ((it->degree) >= minv->degree)
		{
			minv = it;
		}
	}
}

static int share_c1 = 0;
static int share_c2 = 0;

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
	// find graph in sharedGraphNodeMap
	NSTNode* first;
	auto f_it = sharedGraphNodeMap.find(graph);
	if(f_it!=sharedGraphNodeMap.end())
	{
		share_c1++;
		first = f_it->second;
		return first;
	}
	//

	NGraph::ngv_it minv;
	FindMinDegV(graph, minv);
	auto it = minv->edges.begin();

	int ce = *it;
	NGraph* cg = graph;
	first = new NSTNode(ce, cg);
	NSTNode* cn = first;

	auto cn_it = sharedNodeMap.insert(make_pair(cn,cn));
	if(!cn_it.second)
	{
		cout << "share happened here" << endl;
		cn = cn_it.first->second;
		first = cn;
	}
	else
	{
		layer.push(cn);
		nodes.push_back(cn);
	}
/*
	cout << "create node:" << endl;
	cout << cn->eindex << endl;
	cn->graph->Print();
*/

	it++;
	//for (auto et = minv->edges.end(); it != et; it++)
	auto et = minv->edges.end();
	while(it!=et)
	{
		cg = new NGraph(*cg); // new NGraph
		cg->Open(ce);

		// add graph to sharedGraphMap
		auto git = sharedGraphMap.insert(make_pair(cg,cg));
		if(!git.second)
		{
			delete cg;
			cg = git.first->second;
		}

		ce = *it;
		cn->pr = new NSTNode(ce, cg); // new NSTnode

		// add node to sharedNodeMap
		auto nit = sharedNodeMap.insert(make_pair(cn->pr,cn->pr));
		if(!nit.second)
		{
			share_c2++;

			delete (cn->pr);
			cn->pr = nit.first->second;
		}
		else
		{
			layer.push(cn->pr);
			nodes.push_back(cn->pr);
		}

		cn = cn->pr;
		it++;
/*
		cout << "create node:" << endl;
		cout << cn->eindex << endl;
		cn->graph->Print();
*/
	}
	cn->pr = pZeroNSTNode;

	// add first to sharedGraphNodeMap, paired with original graph or the shorted graph
	sharedGraphNodeMap.insert(make_pair(graph,first));
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
	cout << "shared node count: " << share_c1 << "+" << share_c2 << "=" << share_c1+share_c2 << endl;
}

int NSTree::ReleaseNodeGC()
{
	int cnt = 0;
	for (auto it = nodes.begin(), et = nodes.end(); it != et; it++)
	{
		if ((*it)->mark)
		{
			delete (*it);
			nodes.erase(it);
			it--;
			cnt++;
		}
	}
	return cnt;
}


/*
 * zero suppress
 */

void NSTree::ZSuppressNodeR(NSTNode* cn, bool visit)
{
	/*if(cn->eindex==0 || cn->eindex==1)
	 return;
	 */
	if (cn->visit == visit)
		return;
	cn->visit = visit;

	ZSuppressNodeR(cn->pl, visit);
	ZSuppressNodeR(cn->pr, visit);

	NSTNode* node = cn->pl;
	if (node->eindex == 0)
		cn->mark = true;
	else if (node->mark)
		cn->pl = node->pr;
	if (cn->pl->mark)
		cn->mark = true;
	node = cn->pr;
	if (node->mark)
		cn->pr = node->pr;
	return;
}

void NSTree::ZSuppress()
{
	cout << "-- zero suppress begin..." << endl;
	bool visit = !(root->visit);
	pZeroNSTNode->visit = visit;
	pOneNSTNode->visit = visit;
	ZSuppressNodeR(root, visit);
	int cnt = ReleaseNodeGC();
	cout << "- zero suppress done." << endl;
	cout << "zero suppressed node count: " << cnt << endl;

}

void NSTree::ReduceNodeR(NSTNode* cn,bool visit,int& tripleCnt)
{
	if(cn->visit==visit)
		return;
	if(cn->pl->visit!=visit)
		ReduceNodeR(cn->pl,visit,tripleCnt);
	if(cn->pr->visit!=visit)
		ReduceNodeR(cn->pr,visit,tripleCnt);

	if(cn->pl->mark)
		cn->pl = cn->pl->pshare;
	if(cn->pr->mark)
		cn->pr = cn->pr->pshare;

	auto it = sharedTripleMap.insert(make_pair(cn,cn));
	if(it.second)
	{
		cn->tid = tripleCnt++;
	}
	else
	{
		cn->mark = true;
		cn->pshare = it.first->second;
	}

	cn->visit = visit;
	return;
}

void NSTree::Reduce()
{
	cout << "-- reduce begin..." << endl;
	bool visit = !(root->visit);
	pZeroNSTNode->visit = visit;
	pOneNSTNode->visit = visit;
	int tripleCnt = 0;
	pZeroNSTNode->tid = tripleCnt++;
	pOneNSTNode->tid = tripleCnt++;
	ReduceNodeR(root,visit,tripleCnt);
	sharedTripleMap.clear();
	int cnt = ReleaseNodeGC();
	cout << "- reduce done." << endl;
	cout << "reduce node count: " << cnt << endl;
}

void NSTree::Build()
{
	BFSBuild();
	cout << "total node count: " << nodes.size() << endl;
	ZSuppress();
	cout << "total node count: " << nodes.size() << endl;
	Reduce();
	cout << "total node count: " << nodes.size() << endl;
}
