/*
 * edge.h
 *
 *  Created on: Dec 12, 2012
 *      Author: chjd
 */

#ifndef EDGE_H_
#define EDGE_H_

class Edge
{
public:
	int index;
	int vp;
	int vn;
public:
	Edge(int p,int n):vp(p),vn(n){}
	Edge(const Edge& et){
		index = et.index;
		vp = et.vp;
		vn = et.vn;
	}
	void SetIndex(int& k)
	{
		index = k++;
	}
	bool Short(int p,int n)
	{
		if(vp==p)
			vp = n;
		if(vn==p)
			vn = n;
		return vp==vn;
	}
};

#endif /* EDGE_H_ */
