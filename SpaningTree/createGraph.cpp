/*
 * createGraph.cpp
 *
 *  Created on: Dec 24, 2012
 *      Author: chjd
 */

#include "createGraph.h"

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
	//	auto pit = vIndex.insert(make_pair(p, stoi(p)));
		vp = pit.first->second;
		auto nit = vIndex.insert(make_pair(n, (int)vIndex.size()));
	//	auto nit = vIndex.insert(make_pair(n, stoi(n)));
		vn = nit.first->second;
		graph->edges.push_back(Edge(ei, vp, vn));

//		cout << ename << " -> " << ei << ":" << vp << "," << vn << endl;

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




bool CreateOrderGraph(unordered_map<int, list<int> >& vEdges,NGraph* ng,EGraph* eg)
{
	eg->nodenum = vEdges.size();
	list<Edge> tEdges;
	OrderGraph order;
	order.ReOrder(vEdges,ng->edges,tEdges);
//	order.NearMinDegOrder(vEdges,ng->edges,tEdges);
	eg->edges = tEdges;
	eg->edgenum = tEdges.size();
	return true;
}
