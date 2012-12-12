/*
 * ngraph.h
 *
 *  Created on: Dec 11, 2012
 *      Author: chjd
 */

#ifndef NGRAPH_H_
#define NGRAPH_H_

class NGVertex
{
public:
	int vertex;
	int degree;
	list<int> edges;
public:
	void Degree()
	{
		degree = edgeIndexs.size();
	}
	friend bool operator < (const NGVertex& a,const NGVertex& b) const
	{
		return (a.vertex<b.vertex);
	}
};

/*
 * less than operation for NGVertex
 * 	true, a< b
 * 	if:
 * 		a.degree < b.degree
 * 		a.degree == b.degree, a.index < b.index
 */

/*

class NGVertexEqual
{
public:
	bool operator ()(const NGVertex& a,const int v) const
	{
		return a.vertex==v;
	}
};

class NGVertexLessThan
{
public:
	bool operator () (const NGVertex& a,const NGVertex& b) const
	{
		return ((a.edgeIndexs.size()<b.edgeIndexs.size()) ||
				(a.edgeIndexs.size()==b.edgeIndexs.size() && a.vertex<b.vertex));
	}
};

*/


/*
 * Graph
 * 	edge,
 * 	vertex
 */

/*
 * NGraph
 *
 *
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
	NGraph(NGraph& g){
		vertexs = g.vertexs;
		edges = g.edges;
	}
private:
	void FindTwoV(int eindex,es_it& it,int& vp,int& vn)
	{
		// edge not find will not happen
		it = find(edges.begin(), edges.end(), eindex);
		assert(it != edges.end());
		int vp = it->vp;
		int vn = it->vn;
		if (vp > vn)
			swap(vp, vn);
	}

	void MergeEiList(list<int>& vp,list<int>& vn,list<int>& nedges)
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

	void FindTwoNGV(int vp,int vn,ngv_it& vp_it,ngv_it& vn_it)
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

	void ShortAllEdge(int vp,int vn)
	{
		for(auto it=edges.begin(),et=edges.end();it!=et;it++)
		{
			if(it->Short(vp,vn))
			{
				edges.erase(it);
				it--;
			}
		}
	}
public:
	bool Short(int eindex)
	{
		/*
		 * short,
		 * 		find the edge and get vertexs
		 * 		collapse small vertex to large one, and collect loop edge, update degree
		 * 		erase all the loops
		 *
		 */
		es_it e_it;
		int vp,vn;
		FindTwoV(eindex,e_it,vp,vn);
		// vp < vn

		ngv_it vp_it,vn_it;
		FindTwoNGV(vp,vn,vp_it,vn_it);

		// collapse the two vertexs, collapse small to large
		list<int> nedges;
		MergeEiList(vp_it->edges,vn_it->edges,nedges);
		vn_it->edges.swap(nedges);
		vn_it->Degree();
		vertexs->erase(vp_it);

		// if remain only one vertex, then OK.
		if(vertexs.size()==1)
			return true;

		// short on all the edges
		ShortAllEdge(vp,vn);

		return false;
	}
	void Open(int eindex)
	{
		/*
		 * open,
		 * 		find edge, get vertex & erase this edge
		 * 		remove edge in the vertexs
		 */
		es_it e_it;
		int vp,vn;
		FindTwoV(eindex,e_it,vp,vn);

		edges.erase(e_it);

		ngv_it vp_it,vn_it;
		FindTwoNGV(vp,vn,vp_it,vn_it);

		// remove edge on vertex
		vp_it->edges.remove(eindex);
		vp_it->Degree();
		// if isolated vertex, terminate
		if (vp_it.degree == 0)
			return true;

		vn_it->edges.remove(eindex);
		vn_it->Degree();
		if (vn_it->degree == 0)
			return true;
	}
};

#endif /* NGRAPH_H_ */
