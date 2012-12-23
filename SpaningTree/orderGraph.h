/*
 * orderGraph.h
 *
 *  Created on: Dec 21, 2012
 *      Author: chjd
 */

#ifndef ORDERGRAPH_H_
#define ORDERGRAPH_H_

#include <utility>
#include <unordered_map>
#include <list>
#include <vector>
#include <algorithm>
using std::unordered_map;
using std::list;
using std::vector;
using std::pair;
using std::make_pair;

#include <iostream>
using std::cout;
using std::endl;

class OrderGraph
{
public:
	void ReOrder(unordered_map<int,list<int> >& vEdges,list<Edge>& edges,list<Edge>& orderedEdge)
	{
		vector<Edge> edgeV;
		L2V(edges,edgeV);
		int k = 2;
		int ncnt = 0;
		int N = vEdges.size();
		while(ncnt<N-1)
		{
//			PrintVEdge(vEdges);
			unordered_map<int,list<int> >::iterator tit;
			FindMaxDegV(vEdges,tit);
//			cout << "the max degree vertex:" << tit->first << endl;
			vector<pair<int,int> > localOrder;
			for(auto e_it=tit->second.begin(),e_et=tit->second.end();e_it!=e_et;e_it++)
			{
//				cout << "\tprocess egde:" << (*e_it) << endl;
				int ap = AnotherV(edgeV[*e_it],tit->first);
				localOrder.push_back(make_pair(vEdges[ap].size(),*e_it));
				vEdges[ap].remove(*e_it);
			}
			vEdges.erase(tit);
			ncnt++;
			sort(localOrder.begin(),localOrder.end());
			for(auto o_it=localOrder.begin(),o_et=localOrder.end();o_it!=o_et;o_it++)
			{
				Edge& eg = edgeV[o_it->second];
				orderedEdge.push_back(Edge(k++,eg.vp,eg.vn));
			}
		}
	}
	int AnotherV(Edge& edge,int p)
	{
		if(edge.vp==p)
			return edge.vn;
		else
			return edge.vp;
	}
	void FindMaxDegV(unordered_map<int,list<int> >& vEdges,unordered_map<int,list<int> >::iterator& tit)
	{
		tit=vEdges.begin();
		int deg = tit->second.size();
		for(auto it=tit,et=vEdges.end();it!=et;it++)
		{
			int cdeg = it->second.size();
			if(cdeg>deg)
			{
				tit = it;
			}
		}
	}
	void L2V(list<Edge>& edges,vector<Edge>& edgeV)
	{
		edgeV.push_back(Edge(0,1,0));
		edgeV.push_back(Edge(1,1,0));
		for(auto it=edges.begin(),et=edges.end();it!=et;it++)
		{
			edgeV.push_back(*it);
		}
	}
	void PrintVEdge(unordered_map<int,list<int> >& vEdges)
	{
		for(auto it=vEdges.begin(),et=vEdges.end();it!=et;it++)
		{
			cout << it->first << ":";
			for(auto e_it=it->second.begin(),e_et=it->second.end();e_it!=e_et;e_it++)
			{
				cout << (*e_it) << ",";
			}
			cout << endl;
		}
	}
};


#endif /* ORDERGRAPH_H_ */
