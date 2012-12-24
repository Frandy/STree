/*
 * createGraph.h
 *
 *  Created on: Dec 24, 2012
 *      Author: chjd
 */

#ifndef CREATEGRAPH_H_
#define CREATEGRAPH_H_

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


#endif /* CREATEGRAPH_H_ */
