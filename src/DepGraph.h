#ifndef DEPGRAPH_H
#define DEPGRAPH_H

#include "Rule.hpp"
#include "RuleList.h"
#include "Node.h"
#include <cmath>
#include <set>
#include <deque>
#include <list>
class DepGraph{
  public:
    node * root;
		std::vector<node*> Nodes;
		rule_list * rList;
    uint32_t thres_soft;
    uint32_t thres_hard;
    uint32_t total_rules;

  public:
		DepGraph();
    DepGraph(rule_list &); //Constructor
    ~DepGraph(); //Destructor
		void delNode(node *);
		//bool isReachable(vector<uint32_t>,vector<uint32_t>)
		bool isReachable(uint32_t,uint32_t);

  private:
};

#endif


