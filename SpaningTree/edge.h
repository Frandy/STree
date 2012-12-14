/*
 * edge.h
 *
 *  Created on: Dec 12, 2012
 *      Author: chjd
 */

#ifndef EDGE_H_
#define EDGE_H_

#include <iostream>
using std::ostream;

class Edge
{
public:
	int index;
	int vp;
	int vn;
public:
	Edge(int p,int n):vp(p),vn(n){}
	Edge(int e,int p,int n):index(e),vp(p),vn(n){}
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
	bool ShortReIndex(int p,int n)
	{
		if(vp==p)
			vp = n;
		else if(vp>p)
			vp -= 1;
		if(vn==p)
			vn = n;
		else if(vn>p)
			vn -= 1;
		return vp==vn;
	}

	friend ostream& operator << (ostream& out,const Edge& et)
	{
		out << et.index << ":" << et.vp << "," << et.vn << " ";
		return out;
	}
};

#endif /* EDGE_H_ */
