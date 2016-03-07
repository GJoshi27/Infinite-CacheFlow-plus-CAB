#include "Node.h"


using namespace std;
#include<vector>

node::node(uint32_t rid){
	sonList = vector<node*>();
	parentList= vector<node*>();
	related_rules=vector<uint32_t>();
	rule_id= rid;
}

