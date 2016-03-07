#ifndef DEPGRAPH_H
#define DEPGRAPH_H

#include "RuleList.h"
#include "Node.h"
class DepGraph{
	public:
		Node * root;
		std::vector<Node*> Nodes;
		Rule_List * rList;
		uint32_t thres_soft;
		uint32_t thres_hard;
		uint32_t total_rules;

	public:
		DepGraph();
		DepGraph(Rule_List &); //Constructor
		bool isReachable(uint32_t,uint32_t);

};

#endif


