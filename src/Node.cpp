#include "Node.h"


using namespace std;
#include<vector>

Node::Node(uint32_t rid){
	sonList = vector<Node*>();
	parentList= vector<Node*>();
	related_rules=vector<uint32_t>();
	rule_id= rid;
}

