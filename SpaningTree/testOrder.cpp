/*
 * testOrder.cpp
 *
 *  Created on: Dec 24, 2012
 *      Author: chjd
 */

#include <fstream>
using std::ifstream;
#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <unordered_map>
using std::unordered_map;

#include "nstree.h"
#include "estree.h"

#include "orderGraph.h"

#include "genGraph.h"

bool CreateNGraph(string name, NGraph* graph,unordered_map<int, list<int> >& vEdges);
bool CreateEGraph(NGraph* ng,EGraph* eg);

bool CreateOrderGraph(unordered_map<int, list<int> >& vEdges,NGraph* ng,EGraph* eg);

int main(int argc,char** argv)
{
	string name = "rndg.txt";

/*	GenGraph genGraph;
	genGraph.Run();
*/
	/* test nstree */
	NGraph* graph = new NGraph;
	unordered_map<int, list<int> > vEdges;
	bool ok = CreateNGraph(name,graph,vEdges);
	if(!ok)
	{
		cout << "failed to create graph, exit" << endl;
		return -1;
	}
	graph->Print();

	EGraph* orderEg = new EGraph;
	orderEg->Print();
	CreateOrderGraph(vEdges,graph,orderEg);
	ESTree* orderEst = new ESTree(orderEg);
	orderEst->Build();
	orderEst->PrintAllPath();

	delete orderEst;
	delete orderEg;

//	delete stree;
	delete graph;

	return 0;
}

