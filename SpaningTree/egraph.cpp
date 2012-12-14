/*
 * egraph.cpp
 *
 *  Created on: Dec 14, 2012
 *      Author: chjd
 */

#include <algorithm>
using std::find_if;
using std::swap;
using std::min;
#include <cassert>

#include <iostream>
using std::cout;
using std::endl;

#include <string>

#include "egraph.h"

void EGraph::FindTwoV(int eindex, es_it& it, int& vp, int& vn)
{
	it = find_if(edges.begin(), edges.end(), [=](Edge& v)->bool{return v.index==eindex;});
	if (it == edges.end())
		return;
	vp = it->vp;
	vn = it->vn;
	if (vp > vn)
		swap(vp, vn);
}
void EGraph::ShortAllEdge(int vp, int vn)
{
	for (auto it = edges.begin(), et = edges.end(); it != et; it++)
	{
		if (it->ShortReIndex(vp, vn))
		{
			edges.erase(it);
			it--;
		}
	}
}

// for hash, graph share
size_t EGraph::Hash() const
{
	// 4 8-bit
	int seed_a = nodenum * 7 + edgenum;
	int prime = 37;
	int seed_b = 0, seed_c = 0, seed_d = 0;
	for (auto it = edges.begin(), et = edges.end(); it != et; it++)
	{
		seed_b ^= it->index * (seed_c % prime);
		seed_c |= it->vp * (seed_d % prime);
		seed_d &= it->vn * (seed_b % prime);
	}
	size_t seed = (seed_d << 24) & (0xFF000000);
	seed = seed | ((seed_c << 16) & (0x00FF0000));
	seed = seed | ((seed_b << 8) & (0x0000FF00));
	seed = seed | (seed_a & 0x000000FF);
	return seed;
}

// another hash policy, each edge to a char, then hash string
// use char16_t and u16string, or just char, string
// char16_t, 65536
// char, 256
size_t EGraph::HashByString() const
{
	std::string seed_t(min(edgenum + 1, 16), char(0));
	seed_t[0] = char(nodenum + edgenum * 7);
	int i = 1;
	for (auto it = edges.begin(), et = edges.end(); it != et; it++)
	{
		int t = it->index * 11 + it->vp * 5 + it->vn;
		seed_t[i++] = char(t);
		if (i > 16)
			break;
	}
	std::hash<std::string> hash_fn;
	return hash_fn(seed_t);
}

void EGraph::Print()
{
	cout << "Edges:" << edges.size() << endl;
	for(auto it=edges.begin(),et=edges.end();it!=et;it++)
	{
		cout << (*it) << "\t";
	}
	cout << endl;
}

int EGraph::Short(int eindex)
{
	es_it e_it;
	int vp, vn;
	FindTwoV(eindex, e_it, vp, vn);
	// no target edge found
	if (e_it == edges.end())
		return 0;

	nodenum -= 1;
	// short large to small
	ShortAllEdge(vn, vp);
	edgenum = edges.size();

	// only one node after short
	if (edgenum == 0 && nodenum == 1)
		return 1;
	// confirm it not connect after short, which should be confirmed after open
	else if (edgenum < nodenum - 1)
		return 0;
	else
		return 2;
}
int EGraph::Open(int eindex)
{
	es_it e_it;
	int vp, vn;
	FindTwoV(eindex, e_it, vp, vn);
	// no target edge found
	if (e_it == edges.end())
		return 0;

	edges.erase(e_it);
	edgenum = edges.size();

	// confirm not connect after open
	if (edgenum < nodenum - 1)
		return 0;
	else
		return 2;
}
