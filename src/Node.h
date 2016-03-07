#ifndef _NODE_H
#define _NODE_H
#include "stdafx.h"

class node {
	public:		
		std::vector<node*> sonList; 		// List of son nodes
		std::vector<node*> parentList; 		// List of son nodes
		std::vector<uint32_t> related_rules;	// IDs of related rules in the node
		uint32_t rule_id;

	public:
		node();
		node(uint32_t rid);
};
#endif

