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
#include <utility>
using std::make_pair;

#include <ctime>
using std::clock;
using std::clock_t;

#include "egraph.h"
#include "estree.h"

ESTNode::ESTNode(int e, EGraph* g) :
	eindex(e), graph(g), pl(nullptr), pr(nullptr), visit(false), mark(false)
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
	pZeroESTNode = new ESTNode(0, nullptr);
	pOneESTNode = new ESTNode(1, nullptr);
	pZeroESTNode->pl = pOneESTNode;
	pZeroESTNode->pr = pZeroESTNode;
	pOneESTNode->pl = pOneESTNode;
	pOneESTNode->pr = pZeroESTNode;
	nodes.push_back(pZeroESTNode);
	nodes.push_back(pOneESTNode);
}

void ESTree::PrintAllPath()
{
	list<ESTNode*> paths;
	int cnt = 0;
	cout << "--- all paths begin..." << endl;
	CollectTermR(root, paths,cnt);
	cout << "- all paths done." << endl;
	cout << "path count: " << cnt << endl;
}

void ESTree::PrintTerm(list<ESTNode*>& paths)
{
#ifdef EN_PRINT_TERM
	for (auto it = paths.begin(), et = paths.end(); it != et; it++)
	{
		cout << (*it)->eindex << "\t";
	}
	cout << endl;
#endif
}

void ESTree::CollectTermR(ESTNode* cn, list<ESTNode*>& paths,int& cnt)
{
	if (cn->eindex == 1)
	{
		//cout << "--- cn = 1" << endl;
		PrintTerm(paths);
		cnt++;
	}
	else if (cn->eindex == 0)
	{
		//cout << "... cn = 0" << endl;
		return;
	}
	else
	{
		//cout << "push in cn:" << cn->eindex << endl;
		paths.push_back(cn);
		CollectTermR(cn->pl, paths,cnt);
	//	cout << "pop cn:" << paths.back()->eindex << endl;
		paths.pop_back();
		CollectTermR(cn->pr, paths,cnt);
	}
}

void ESTree::ReleaseAllGraph()
{
	for (auto it = sharedGraphMap.begin(), et = sharedGraphMap.end(); it != et; it++)
	{
	//	if (it->first == it->second)
		{
			delete (it->second);
			(it->second) = nullptr;
		}
	}
	sharedGraphMap.clear();
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

void ESTree::AddNewNode(int eindex,ESTNode*& node,EGraph*& gl,int& cnt)
{
	auto git = sharedGraphMap.insert(make_pair(gl,gl));
	if(!git.second)
	{
		delete gl;
		gl = git.first->second;
	}

	node = new ESTNode(eindex, gl);
	auto nit = sharedNodeMap.insert(make_pair(node,node));
	if(nit.second)
	{
		layer.push_back(node);
		nodes.push_back(node);
	}
	else
	{
//		cout << "sharing node:" << node->eindex << "\t:"<< node << endl;
		delete (node);
		cnt++;
		node = nit.first->second;
	}
}

void ESTree::BFSBuild()
{
	cout << "---BFS build begin..." << endl;

	root = new ESTNode(origin->edges.front().index, origin);
//	origin->Print();
	layer.push_back(root);
	nodes.push_back(root);
	int cnt=0;	// cnt share node during construct
	while (!layer.empty())
	{
		ESTNode* cn = layer.front();

		// short eindex to get pl
		EGraph* gl = new EGraph(*(cn->graph));
		int okl = gl->Short(cn->eindex);
		if (okl == 1)
			cn->pl = pOneESTNode;
		else if (okl == 0)
			cn->pl = pZeroESTNode;
		else
		{
			AddNewNode(cn->eindex+1,cn->pl,gl,cnt);
		}

		// open eindex to get pr
		EGraph* gr = new EGraph(*(cn->graph));
		int okr = gr->Open(cn->eindex);
		if (okr == 0)
			cn->pr = pZeroESTNode;
		else
		{
			AddNewNode(cn->eindex+1,cn->pr,gr,cnt);
		}
/*
		 cout << "-- node: " << cn->eindex << "\t" << cn << ":" << cn->graph << endl;
		 cn->graph->Print();
		 cout << "\tpl:" << cn->pl->eindex << "\t:" << cn->pl << endl;
		 cout << "\tpr:" << cn->pr->eindex << "\t:" << cn->pr << endl;
*/
		layer.pop_front();
	}
	ReleaseAllGraph();
	sharedNodeMap.clear();
	cout << "-BFS build done." << endl;
	cout << "shared node count: " << cnt << endl;
}

void ESTree::DFSBuild()
{
	// use layer as stack
	cout << "---DFS build begin..." << endl;
	root = new ESTNode(origin->edges.front().index, origin);
	layer.push_back(root);
	nodes.push_back(root);
	while (!layer.empty())
	{
		ESTNode* cn = layer.back();
		layer.pop_back();
		// short eindex to get pl
		EGraph* gl = new EGraph(*(cn->graph));
		int okl = gl->Short(cn->eindex);
		if (okl == 1)
			cn->pl = pOneESTNode;
		else if (okl == 0)
			cn->pl = pZeroESTNode;
		else
		{
			cn->pl = new ESTNode(cn->eindex + 1, gl);
			nodes.push_back(cn->pl);
		}

		// open eindex to get pr
		EGraph* gr = new EGraph(*(cn->graph));
		int okr = gr->Open(cn->eindex);
		if (okr == 0)
			cn->pr = pZeroESTNode;
		else
		{
			cn->pr = new ESTNode(cn->eindex + 1, gr);
			nodes.push_back(cn->pr);
		}
		if(okr!=0)
			layer.push_back(cn->pr);
		if(okl!=1 && okl!=0)
			layer.push_back(cn->pl);
/*
		 cout << "-- node: " << cn->eindex << endl;
		 cn->graph->Print();
		 cout << "\tpl:" << cn->pl->eindex << "\t pr:" << cn->pr->eindex << endl;
*/
	}
	sharedNodeMap.clear();
	cout << "---DFS build done..." << endl;

}


int ESTree::ReleaseNodeGC()
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

void ESTree::ZSuppressNodeR(ESTNode* cn, bool visit)
{
	/*if(cn->eindex==0 || cn->eindex==1)
	 return;
	 */
	if (cn->visit == visit)
		return;
	cn->visit = visit;

	ZSuppressNodeR(cn->pl, visit);
	ZSuppressNodeR(cn->pr, visit);

	ESTNode* node = cn->pl;
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

void ESTree::ZSuppress()
{
	cout << "-- zero suppress begin..." << endl;
	bool visit = !(root->visit);
	pZeroESTNode->visit = visit;
	pOneESTNode->visit = visit;
	ZSuppressNodeR(root, visit);
	int cnt = ReleaseNodeGC();
	cout << "- zero suppress done." << endl;
	cout << "zero suppressed node count: " << cnt << endl;

}

void ESTree::ReduceNodeR(ESTNode* cn,bool visit,int& tripleCnt)
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

void ESTree::Reduce()
{
	cout << "-- reduce begin..." << endl;
	bool visit = !(root->visit);
	pZeroESTNode->visit = visit;
	pOneESTNode->visit = visit;
	int tripleCnt = 0;
	pZeroESTNode->tid = tripleCnt++;
	pOneESTNode->tid = tripleCnt++;
/*	int mint = origin->nodenum;
	if(mint>32)
		mint = 20;
	else if(mint>20)
		mint = 14;
	mint = 2 << mint;
	sharedTripleMap = SharedTripleMapT(mint);
*/
	ReduceNodeR(root,visit,tripleCnt);
//	cout << "triple load factor:" << sharedTripleMap.load_factor() << endl;
	sharedTripleMap.clear();
	int cnt = ReleaseNodeGC();
	cout << "- reduce done." << endl;
	cout << "reduce node count: " << cnt << endl;
}

void ESTree::Build()
{
	clock_t t0 = clock();
	BFSBuild();
//	DFSBuild();
	cout << "total node count: " << nodes.size() << endl;
	clock_t t1 = clock();
	ZSuppress();
	cout << "total node count: " << nodes.size() << endl;
	clock_t t2 = clock();
	Reduce();
	cout << "total node count: " << nodes.size() << endl;
	clock_t t3 = clock();
	cout << "timing statistics:" << endl;
	cout << "\tbuild:\t" << (double(t1-t0)/CLOCKS_PER_SEC) << endl;;
	cout << "\tzsuppress:\t" << (double(t2-t1)/CLOCKS_PER_SEC) << endl;
	cout << "\treduce:\t" << (double(t3-t2)/CLOCKS_PER_SEC) << endl;
	cout << "\ttotal time:\t" << (double(t3-t0)/CLOCKS_PER_SEC) << endl;
}
