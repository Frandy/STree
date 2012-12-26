/*
 * estree.h
 *
 *  Created on: Dec 14, 2012
 *      Author: chjd
 */

#ifndef ESTREE_H_
#define ESTREE_H_

#include <list>
using std::list;
#include <queue>
using std::queue;
#include <stack>
using std::stack;
#include <unordered_map>
using std::unordered_map;
#include <map>
using std::map;

#include <string>
using std::u32string;

#include "egraph.h"
class EGraph;

#define TDENSEMAP 1

#if TDENSEMAP
#include <sparsehash/dense_hash_map>
using google::dense_hash_map;
#endif

#define TMAP 0

typedef long long unsigned int UINT64;
typedef unsigned int UINT32;


class ESTNode
{
public:
	int eindex;
	EGraph* graph;

	ESTNode* pl;
	ESTNode* pr;

	bool visit;
	bool mark;

	int tid;	// triple index, for reduce
	ESTNode* pshare;	// shared triple,
//	UINT64 tvalue;
	u32string tvalue;

public:
	ESTNode()=default;
	ESTNode(int e, EGraph* g);

	void TripleValue()
	{
/*
		tvalue = ((UINT64(eindex)) << 54) \
						| ((UINT64((UINT32)(pl->tid)&0x08FFFFFF)) << 27) \
						| (UINT64((UINT32)(pl->tid)&0x08FFFFFF));
*/

		tvalue = u32string(3,char32_t(0));
		tvalue[0] = (char32_t(eindex));
		tvalue[1] = (char32_t(pl->tid));
		tvalue[2] = (char32_t(pr->tid));

	}
};

class ENodeHash
{
public:
	size_t operator () (const ESTNode* cn) const
	{
		return (cn->eindex<<24) | (((size_t)(cn->graph)>>4) & 0x00FFFFFF);
	}
};

class ENodeEqual
{
public:
	bool operator () (const ESTNode* a,const ESTNode* b) const
	{
	//	return (a==b)||((*a)==(*b));
		if(a==b)
			return true;
		EGEqual gequal;
		return (a->eindex==b->eindex) && (gequal(a->graph,b->graph));
	}
};

class ETripleHash
{
public:
	std::hash<u32string> hash_fn;
public:
	bool operator () (const ESTNode* cn) const
	{
	/*
		int symbId = cn->eindex;
		int leftId = cn->pl->tid;
		int rightId = cn->pr->tid;
		int al = (int)cn->pl;
		int ar = (int)cn->pr;
		size_t value = 0;
	//	value = (symbId*symbId)+(leftId*leftId<<8)+(rightId*rightId<<16);
		value = (symbId^leftId)<<8;
		value ^= (leftId^rightId);
		value ^= (al << 8) ^ (ar<<2);
	//	value = value&0x7FFFFFFF;
	//	cout << "triple hash value: " << value << endl;
		return value;
	*/
/*
		u32string str(3,char32_t(0));
		str[0] = (char32_t(cn->eindex));
//		str[1] = (char32_t(cn->pl->eindex));
//		str[2] = (char32_t(cn->pr->eindex));
		str[1] = (char32_t(cn->pl->tid));
		str[2] = (char32_t(cn->pr->tid));
//		str[5] = (char32_t(int(cn->pl)));
//		str[6] = (char32_t(int(cn->pr)));
*/
		return hash_fn(cn->tvalue);
	}
};

class ETripleEqual
{
public:
	bool operator () (const ESTNode* a,const ESTNode* b) const
	{
		return (a->eindex==b->eindex &&
				a->pl->tid==b->pl->tid &&
				a->pr->tid==b->pr->tid);
	}
};

class ETripleNEqual
{
public:
	bool operator () (const ESTNode* a,const ESTNode* b) const
	{
		return (a->eindex!=b->eindex ||
				a->pl->tid!=b->pl->tid ||
				a->pr->tid!=b->pr->tid);
	}
};

class ETripleLessThan
{
public:
	bool operator () (const ESTNode* a,const ESTNode* b) const
	{
/*		ETripleNEqual tnequal;
		return (a->tvalue < b->tvalue)&&(tnequal(a,b));
*/		/*
		UINT64 ta,tb;
		ta = ((UINT64(a->eindex)) << 54) \
				| ((UINT64((UINT32)(a->pl->tid)&0x08FFFFFF)) << 27) \
				| (UINT64((UINT32)(a->pl->tid)&0x08FFFFFF));
		tb = ((UINT64(b->eindex)) << 54) \
				| ((UINT64((UINT32)(b->pl->tid)&0x08FFFFFF)) << 27) \
				| (UINT64((UINT32)(b->pl->tid)&0x08FFFFFF));
		return ta<tb;
		*/
/*
		if(a->eindex<b->eindex)
			return true;
		else if(a->pl->tid<b->pl->tid)
			return true;
		else if(a->pr->tid<b->pl->tid)
			return true;
		return false;
*/
		return a->tvalue < b->tvalue;
	}
};

class ESTree
{
public:
	EGraph* origin; // the original graph
	ESTNode* root; // root node
	list<ESTNode*> nodes; // all nodes
private:
	ESTNode* pZeroESTNode; // leaf node, zero & one
	ESTNode* pOneESTNode;
	list<ESTNode*> layer; // working layer

	// for sharing
#if TDENSEMAP
	dense_hash_map<EGraph*,EGraph*,EGHash,EGEqual> sharedGraphMap;
	dense_hash_map<ESTNode*,ESTNode*,ENodeHash,ENodeEqual> sharedNodeMap;
#else
	unordered_map<EGraph*,EGraph*,EGHash,EGEqual> sharedGraphMap;
	unordered_map<ESTNode*,ESTNode*,ENodeHash,ENodeEqual> sharedNodeMap;
#endif
#if TMAP
	typedef map<ESTNode*,ESTNode*,ETripleLessThan> SharedTripleMapT;
#elif TDENSEMAP
	typedef dense_hash_map<ESTNode*,ESTNode*,ETripleHash,ETripleEqual> SharedTripleMapT;
#else
	typedef unordered_map<ESTNode*,ESTNode*,ETripleHash,ETripleEqual> SharedTripleMapT;
#endif


	SharedTripleMapT sharedTripleMap;

public:
	// construct
	ESTree()=default;
	ESTree(EGraph* g);
	~ESTree();
	// build & zero suppress & reduce
	void Build();

	// print
	void PrintAllPath();

private:
	void PrintTerm(list<ESTNode*>& paths);
	void CollectTermR(ESTNode* cn, list<ESTNode*>& paths,int& cnt);

	void AddNewNode(int eindex,ESTNode*& node,EGraph*& gl,int& cnt);

	void BFSBuild();
	void DFSBuild();
	void ZSuppress();
	void Reduce();
	void ZSuppressNodeR(ESTNode* cn,bool visit);
	void ReduceNodeR(ESTNode* cn,bool visit,int& tripleCnt);
	int ReleaseNodeGC();

	// non-recursive reduce
	void ReduceN();
	void ReduceNodeN(ESTNode* cn,bool visit,int& tripleCnt);

	void InitZeroOne();
	// release memory of graph & node
	void ReleaseAllGraph();
	void ReleaseAllNode();
};

#endif /* ESTREE_H_ */
