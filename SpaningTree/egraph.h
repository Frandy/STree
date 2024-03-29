/*
 * egraph.h
 *
 *  Created on: Dec 11, 2012
 *      Author: chjd
 */

#ifndef EGRAPH_H_
#define EGRAPH_H_

#include <list>
using std::list;

#include "edge.h"

class EGVertex
{

};

class EGraph
{
public:
	list<Edge> edges;
	int nodenum;
	int edgenum;

	typedef list<Edge>::iterator es_it;
private:
	void FindTwoV(int eindex,es_it& it,int& vp,int& vn);
	void ShortAllEdge(int vp,int vn);
public:
	// for hash, graph share
	size_t Hash() const;

	// another hash policy, each edge to a char, then hash string
	// use char16_t and u16string, or just char, string
	// char16_t, 65536
	// char, 256
	size_t HashByString() const;
	friend bool operator == (const EGraph& a,const EGraph& b);


	void Print();

	int Short(int eindex);
	int Open(int eindex);
};

class EGHash
{
public:
	size_t operator () (const EGraph* g) const
	{
		//return g->Hash();
		return g->HashByString();
	}
};

class EGEqual
{
public:
	bool operator () (const EGraph* a,const EGraph* b) const
	{
		return (a==b) || (*a)==(*b);
	}
};

#endif /* EGRAPH_H_ */
