/*
 * ngraph.h
 *
 *  Created on: Dec 11, 2012
 *      Author: chjd
 */

#ifndef NGRAPH_H_
#define NGRAPH_H_

#include <list>
using std::list;

#include "edge.h"

class NGVertex;
class NGraph;

class NGVertex
{
public:
	int vertex;
	int degree;
	list<int> edges;
public:
	NGVertex()=default;
	// update degree
	void Degree();
};

/*
 * NGraph
 *
 */


class NGraph
{
public:
	list<NGVertex> vertexs;
	list<Edge> edges;

	typedef list<NGVertex>::iterator ngv_it;
	typedef list<Edge>::iterator es_it;
public:
	// default construct
	NGraph()=default;
	// copy construct
	NGraph(NGraph& g);
public:
	// short & open operation for the graph
	bool Short(int eindex);
	bool Open(int eindex);
private:
	// basic operation for short & open
	// find the target edge & its two vertex
	void FindTwoV(int eindex,es_it& it,int& vp,int& vn);

	// merge two edgei list when collapse two node, loops except
	void MergeEiList(list<int>& vp,list<int>& vn,list<int>& nedges);

	// find the two vertex in vertexs
	void FindTwoNGV(int vp,int vn,ngv_it& vp_it,ngv_it& vn_it);

	// short the two node for all edges
	void ShortAllEdge(int vp,int vn);
};

#endif /* NGRAPH_H_ */
