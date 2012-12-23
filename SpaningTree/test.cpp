/*
 * test.cpp
 *
 *  Created on: Dec 12, 2012
 *      Author: chjd
 */

#include <fstream>
using std::ifstream;
#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <unordered_map>
using std::unordered_map;

#include "nstree.h"
#include "estree.h"

#include "orderGraph.h"


bool CreateNGraph(string name, NGraph* graph,unordered_map<int, list<int> >& vEdges);
bool CreateEGraph(NGraph* ng,EGraph* eg);

bool CreateOrderGraph(unordered_map<int, list<int> >& vEdges,NGraph* ng,EGraph* eg)
{
	eg->nodenum = vEdges.size();
	list<Edge> tEdges;
	OrderGraph order;
	order.ReOrder(vEdges,ng->edges,tEdges);
	eg->edges = tEdges;
	eg->edgenum = tEdges.size();
	return true;
}

int main(int argc,char** argv)
{
	string name = "tg.txt";
	if(argc==2)
	{
		name = argv[1];
	}
	else
	{
		cout << "usage: stree tg.txt" << endl;
		return -1;
	}

	/* test nstree */
	NGraph* graph = new NGraph;
	unordered_map<int, list<int> > vEdges;
	bool ok = CreateNGraph(name,graph,vEdges);
	if(!ok)
	{
		cout << "failed to create graph, exit" << endl;
		return -1;
	}
	graph->Print();

	NSTree* stree = new NSTree(graph);
	stree->Build();
//	stree->BFSBuild();

	stree->PrintAllPath();

	/* test estree */
	EGraph* eg = new EGraph;
	ok = CreateEGraph(graph,eg);
//	eg->Print();
	if(!ok)
	{
		cout << "failed to create egraph" << endl;
		return -1;
	}
	ESTree* est = new ESTree(eg);
	est->Build();
	est->PrintAllPath();

	EGraph* orderEg = new EGraph;
	CreateOrderGraph(vEdges,graph,orderEg);
//	orderEg->Print();
//	cout << orderEg->edgenum << endl;
//	cout << orderEg->nodenum << endl;
	ESTree* orderEst = new ESTree(orderEg);
	orderEst->Build();
	stree->PrintAllPath();

	delete orderEst;
	delete orderEg;

	delete stree;
	delete graph;

	delete est;
	delete eg;

	return 0;
}

bool CreateNGraph(string name, NGraph* graph,unordered_map<int, list<int> >& vEdges)
{
	ifstream fp(name.c_str());
	string ename, p, n;
	int ei, vp, vn;
	unordered_map<string, int> eIndex;
	unordered_map<string, int> vIndex;
//	unordered_map<int, list<int> > vEdges;
	int lcnt = 0;
	while (true)
	{
		fp >> ename >> p >> n;
		if (ename.empty())
			break;
		lcnt++;
		auto eit = eIndex.insert(make_pair(ename, 2 + eIndex.size()));
		if (eit.second)
			ei = eit.first->second;
		else
		{
			cerr << "error input at line " << lcnt << ": " << ename << endl;
			return false;
		}
		auto pit = vIndex.insert(make_pair(p, (int)vIndex.size()));
		vp = pit.first->second;
		auto nit = vIndex.insert(make_pair(n, (int)vIndex.size()));
		vn = nit.first->second;
		graph->edges.push_back(Edge(ei, vp, vn));

		cout << ename << " -> " << ei << ":" << vp << "," << vn << endl;

		auto pe_it = vEdges.find(vp);
		if (pe_it == vEdges.end())
		{
			list<int> tmp;
			tmp.push_back(ei);
			vEdges.insert(make_pair(vp, tmp));
		}
		else
		{
			pe_it->second.push_back(ei);
		}

		auto ne_it = vEdges.find(vn);
		if (ne_it == vEdges.end())
		{
			list<int> tmp;
			tmp.push_back(ei);
			vEdges.insert(make_pair(vn, tmp));
		}
		else
		{
			ne_it->second.push_back(ei);
		}
		ename = p = n = "";
	}
	for(int i=0,N=vEdges.size();i<N;i++)
	{
		graph->vertexs.push_back(NGVertex(i,vEdges[i]));
	}

	return true;
}

bool CreateEGraph(NGraph* ng,EGraph* eg)
{
	eg->edges = ng->edges;
	eg->nodenum = ng->vertexs.size();
	eg->edgenum = eg->edges.size();
	return true;
}
