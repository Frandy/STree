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
	/*
		Dis disv(5, 8);
		tvnum = disv(gen);
		int emin = tvnum*2;
		int emax = tvnum*(tvnum-1)/2;
		Dis dise(emin,emax);
		tenum = dise(gen);
	*/
		Dis disv(16, 32);
		tvnum = disv(gen);
		// sparse
		Dis dist(5, 10);
		int sparse = dist(gen);
		tenum = tvnum * (tvnum - 1) / 2 * sparse / 100;
		if(tenum<tvnum || tenum>3*tvnum)
			tenum = tvnum * 2 - 8;
		// sparse min = 1/tvnum * 100%

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
		bool connect = false;
		do{
			GenAllEdge(eArray);
			connect = CheckConnect(eArray);
		}while(!connect);
		SaveAllEdge(eArray);
//		cout << "vnum:" << tvnum << "\t" << "enum:" << tenum << endl;
		cout << "generate rndg done." << endl;
	}
	bool CheckConnect(list<Edge>& eArray)
	{
		unordered_map<int,list<int> > connectMap;
		unordered_map<int,bool> visitMap;
		for(auto it=eArray.begin(),et=eArray.end();it!=et;it++)
		{
			int vp = it->vp;
			int vn = it->vn;
			auto t_it = connectMap.find(vp);
			if(t_it==connectMap.end())
			{
				list<int> tmp;
				tmp.push_back(vn);
				connectMap.insert(make_pair(vp,tmp));
			}
			else
			{
				t_it->second.push_back(vn);
			}

			t_it = connectMap.find(vn);
			if (t_it == connectMap.end())
			{
				list<int> tmp;
				tmp.push_back(vp);
				connectMap.insert(make_pair(vn, tmp));
			}
			else
			{
				t_it->second.push_back(vp);
			}

			visitMap[vp] = false;
			visitMap[vn] = false;
		}
		int start = connectMap.begin()->first;
		list<int> layer;
		layer.push_back(start);
		while(!layer.empty())
		{
			int top = layer.front();
			visitMap[top] = true;
			list<int>& toVisit = connectMap[top];
			for(auto v_it=toVisit.begin(),v_et=toVisit.end();v_it!=v_et;v_it++)
			{
				if(visitMap[*v_it]==false)
				{
					layer.push_back(*v_it);
					visitMap[*v_it] = true;
				}
			}
			layer.pop_front();
		}
		bool flag = true;
		for(auto v_it=visitMap.begin(),v_et=visitMap.end();v_it!=v_et;v_it++)
		{
			if(v_it->second!=true)
				flag = false;
		}
		if(true)
		{
			tvnum = visitMap.size();
		}
		return flag;
	}
};

/*
class GenSparseGraph
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
		Dis disv(16, 32);
		tvnum = disv(gen);
		// sparse
		Dis dist(5,10);
		int sparse = dist(gen);
		tenum = tvnum*(tvnum-1)/2 * sparse / 100;
		// sparse min = 1/tvnum * 100%
	}
	void GenEdge(vector<int>& vArray,Dis& dis,Gen& gen,vector<int>& eRec,list<Edge>& eArray)
	{

	}
};
*/


#endif /* GENGRAPH_H_ */
