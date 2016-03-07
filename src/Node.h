#ifndef _NODE_H
#define _NODE_H
#include "stdafx.h"

class Node {
	public:		
		std::vector<Node*> sonList; 		// List of son nodes
		std::vector<Node*> parentList; 		// List of son nodes
		std::vector<uint32_t> related_rules;	// IDs of related rules in the node
		uint32_t rule_id;

	public:
		Node();
		Node(uint32_t rid);
};
#endif

