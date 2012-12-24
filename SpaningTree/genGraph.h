/*
 * genGraph.h
 *
 *  Created on: Dec 24, 2012
 *      Author: chjd
 */

#ifndef GENGRAPH_H_
#define GENGRAPH_H_

#include <vector>
#include <list>
#include <fstream>
#include <random>
using std::vector;
using std::list;
using std::ofstream;
using std::endl;

class GenGraph
{
public:
	typedef std::uniform_int_distribution<> Dis;
	typedef std::mt19937 Gen;
	int tvnum;
	int tenum;
public:
	void Init()
	{
		std::random_device rd;
		Gen gen(rd());
		Dis disv(5, 8);
		tvnum = disv(gen);
		int emin = tvnum*2;
		int emax = tvnum*(tvnum-1)/2;
		Dis dise(emin,emax);
		tenum = dise(gen);
	}
	void GenEdge(vector<int>& vArray,Dis& dis,Gen& gen,vector<int>& eRec,list<Edge>& eArray)
	{
		int ta = dis(gen);
		int tb = dis(gen);
		while(ta==tb)
		{
			tb = dis(gen);
		}
		int a = vArray[ta];
		int b = vArray[tb];
		if(eRec[a*tvnum+b]!=1)
		{
			eArray.push_back(Edge(eArray.size(),a,b));
			eRec[a*tvnum+b] = 1;
			eRec[b*tvnum+a] = 1;
		}
	}
	void GenAllEdge(list<Edge>& eArray)
	{
		vector<int> vArray(tvnum);
		for(int i=0;i<tvnum;i++)
		{
			vArray[i] = i;
		}
		vector<int> eRec(tvnum*tvnum,0);

		std::random_device rd;
		Gen tgen(rd());
		Dis tdis(0,tvnum-1);

		while(true)
		{
			GenEdge(vArray,tdis,tgen,eRec,eArray);
			if(int(eArray.size())==tenum)
				break;
		}
	}
	void SaveAllEdge(list<Edge>& eArray)
	{
		ofstream fp("rndg.txt");
		cout << "target vnum:" << tvnum << "\tenum:" << tenum << endl;
		for(auto it=eArray.begin(),et=eArray.end();it!=et;it++)
		{
			fp << it->index << " " << it->vp << " " << it->vn << endl;
		}
	}
	void Run()
	{
		list<Edge> eArray;
		Init();
		GenAllEdge(eArray);
		SaveAllEdge(eArray);
//		cout << "vnum:" << tvnum << "\t" << "enum:" << tenum << endl;
		cout << "generate rndg done." << endl;
	}
};


#endif /* GENGRAPH_H_ */
