/*
 * nstree.h
 *
 *  Created on: Dec 11, 2012
 *      Author: chjd
 */

#ifndef NSTREE_H_
#define NSTREE_H_

class NSTNode
{
public:
	int edgeIndex;
	NGraph* graph;

	NSTNode* pl;
	NSTNode* pr;
};

class NSTree
{
public:
	// the original graph
	NGraph* origin;
	// root node
	NSTNode* root;
	// working layer
	queue<NSTNode*> layer;
	unordered_map<NGraph*,NSTNode*> sharedNGraph;
	unordered_map<NSTNode*,NSTNode*> sharedNodeMap;

private:
	// find the vertex with minimum degree
	void FindMinDegV(NGraph* graph,NGraph::ngv_it& minv)
	{
		auto it = graph->vertexs.begin(),et=graph->vertexs.end();
		minv = it++;
		for(;it!=et;it++)
		{
			if((it->degree)<minv->degree)
			{
				minv = it;
			}
		}
	}
public:
	NSTNode* HOpen(NGraph* graph)
	{
		/*
		 * if graph in sharedNGraph,
		 * 		return sharedNGraph[graph]
		 * to built horizontal,
		 *		find the minimal degree node,
		 *		//open edges on this node,
		 *		//	the first node, <= the first edge & the graph
		 *		//	other nodes, <= edge & the open-previous-edge graph
		 *
		 *		for edge in this node,
		 *			if edge & graph pair in sharedNodeMap,
		 *				share the left, and do not push into queue,
		 *			else,
		 *				create NSTNode, push into queue and sharedNodeMap
		 *				hash graph and node,
		 *				hash edge & graph pair and node
		 *			graph open edge
		 *	return the first node, to be the left
		 *
		 */
		NGraph::ngv_it minv;
		FindMinDegV(graph,minv);
		auto it = minv->edges.begin();

		int ce = *it;
		NGraph* cg = graph;
		NSTNode* first = new NSTNode(ce,cg);
		NSTNode* cn = first;

		it++;
		for(auto et=minv->edges.end();it!=et;it++)
		{
			cg = new NGraph(*cg);			// new NGraph
			cg->Open(ce);
			ce = *it;
			cn->pr = new NSTNode(ce,cg);	// new NSTnode
			cn = cn->pr;
		}
		cn->pr = pZeroNSTNode;

		return first;
	}
	void VShort(NSTNode* node)
	{
		/*
		 * if node.graph short node.edge => 1
		 * 		node->pl = 1
		 * 	else
		 * 		node->pl = HOpen(node.graph)
		 */
		NGraph* cg = new NGraph(*node.graph);
		if(cg->Short(node.eindex))
		{
			node->pl = pOneNSTNode;
			delete cg;
		}
		else
		{
			node->pl = HOpen(cg);
		}
	}
	void BFSBuild()
	{
		root = HOpen(origin);
		while(!queue.empty())
		{
			node = queue.front();
			VShort(node);
			queue.pop();
		}
	}
};

#endif /* NSTREE_H_ */
