/*
 * ngraph.cpp
 *
 *  Created on: Dec 12, 2012
 *      Author: chjd
 */

#include <algorithm>
using std::find_if;
using std::swap;
#include <cassert>

#include <iostream>
using std::cout;
using std::endl;


#include "ngraph.h"


void NGVertex::Degree()
{
	degree = edges.size();
}

NGraph::NGraph(NGraph& g)
{
	vertexs = g.vertexs;
	edges = g.edges;
}

void NGraph::Print()
{
	cout << "Edges:" << edges.size() << endl;
	for(auto it=edges.begin(),et=edges.end();it!=et;it++)
	{
		cout << (*it) << "\t";
	}
	cout << endl;
	cout << "Vertexs:" << vertexs.size() << endl;
	for(auto it=vertexs.begin(),et=vertexs.end();it!=et;it++)
	{
		cout << (*it) << "\n";
	}
	cout << endl;
}

void NGraph::FindTwoV(int eindex, es_it& it, int& vp, int& vn)
{
	// edge not find will not happen
	it = find_if(edges.begin(), edges.end(), [=](Edge& v)->bool{return v.index==eindex;});
	assert(it != edges.end());
	vp = it->vp;
	vn = it->vn;
	if (vp > vn)
		swap(vp, vn);
}

void NGraph::MergeEiList(list<int>& vp, list<int>& vn, list<int>& nedges)
{
	auto e_vp_it = vp.begin(), e_vp_et = vp.end();
	auto e_vn_it = vn.begin(), e_vn_et = vn.end();
	while (e_vp_it != e_vp_et && e_vn_it != e_vn_et)
	{
		if ((*e_vp_it) < (*e_vn_it))
		{
			nedges.push_back(*e_vp_it);
			e_vp_it++;
		}
		else if ((*e_vp_it) < (*e_vn_it))
		{
			nedges.push_back(*e_vn_it);
			e_vn_it++;
		}
		else
		{
			//eloops.push_back(*e_vp_it);
			e_vp_it++;
			e_vn_it++;
		}
	}
	while (e_vp_it != e_vp_et)
	{
		nedges.push_back(*e_vp_it);
		e_vp_it++;
	}
	while (e_vn_it != e_vn_et)
	{
		nedges.push_back(*e_vn_it);
		e_vn_it++;
	}
}

void NGraph::FindTwoNGV(int vp, int vn, ngv_it& vp_it, ngv_it& vn_it)
{
	for (auto it = vertexs.begin(), et = vertexs.end(); it != et; it++)
	{
		if (it->vertex == vp)
			vp_it = it;
	}
	for (auto it = vp_it, et = vertexs.end(); it != et; it++)
	{
		if (it->vertex == vn)
			vn_it = it;
	}
}

void NGraph::ShortAllEdge(int vp, int vn)
{
	for (auto it = edges.begin(), et = edges.end(); it != et; it++)
	{
		if (it->Short(vp, vn))
		{
			edges.erase(it);
			it--;
		}
	}
}

bool NGraph::Short(int eindex)
{
	/*
	 * short,
	 * 		find the edge and get vertexs
	 * 		collapse small vertex to large one, and collect loop edge, update degree
	 * 		erase all the loops
	 *
	 */
	es_it e_it;
	int vp, vn;
	FindTwoV(eindex, e_it, vp, vn);
	// vp < vn

	ngv_it vp_it, vn_it;
	FindTwoNGV(vp, vn, vp_it, vn_it);

	// collapse the two vertexs, collapse small to large
	list<int> nedges;
	MergeEiList(vp_it->edges, vn_it->edges, nedges);
	vn_it->edges.swap(nedges);
	vn_it->Degree();
	vertexs.erase(vp_it);

	// if remain only one vertex, then OK.
	if (vertexs.size() == 1)
		return true;

	// short on all the edges
	ShortAllEdge(vp, vn);

	return false;
}

bool NGraph::Open(int eindex)
{
	/*
	 * open,
	 * 		find edge, get vertex & erase this edge
	 * 		remove edge in the vertexs
	 */
	es_it e_it;
	int vp, vn;
	FindTwoV(eindex, e_it, vp, vn);

	edges.erase(e_it);

	ngv_it vp_it, vn_it;
	FindTwoNGV(vp, vn, vp_it, vn_it);

	// remove edge on vertex
	vp_it->edges.remove(eindex);
	vp_it->Degree();
	// if isolated vertex, terminate
	if (vp_it->degree == 0)
		return true;

	vn_it->edges.remove(eindex);
	vn_it->Degree();
	if (vn_it->degree == 0)
		return true;
	return false;
}
