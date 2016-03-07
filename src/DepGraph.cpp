#include "DepGraph.h"

// ---------- bucket_tree ------------
DepGraph::DepGraph() {
	root = NULL;
}

DepGraph::DepGraph(Rule_List & rL) {
	root = new node(0); /* full address space for root node Root node marked as
			       node with id as 0 */
	printf("Creating Dependency Graph \n");  
	uint32_t i=0; 
	int32_t j=0;
	uint32_t sz;
	for (i = 0; i <= rL.list.size(); i++){
		/*  Nodes[0] is dummy and may be later used as root */
		node *tmp=new node(i);
		Nodes.insert(Nodes.end(),tmp);
	}

	//		printf("dep_map size: %ld \n",rL.dep_map.size());
	for(j=rL.dep_map.size()-1;j>=0;j--){
		sz=root->sonList.size();
		if(rL.dep_map[j].size()==0)
		{
			printf("%d has no adjacents \n",j+1);
			root->sonList.insert(root->sonList.end(),Nodes[j+1]);
			Nodes[j+1]->related_rules.insert(Nodes[j+1]->related_rules.end(),0);
		}
		else{
			/*   for each adjacent of node check 
			 *   whether they are connected or not directly/indirectly if connected
			 *   then don't need to do anything else connect them 
			 *   write a method called as isReachable ()*/
			uint32_t Rid;
			for (i=0;i<rL.dep_map[j].size();i++)
			{
				Rid=rL.dep_map[j][i];
				if(!isReachable(j+1,Rid+1))
				{
					printf("%d is not reachable through %d \n",j+1,Rid+1);
					for(uint32_t k=0;k<Nodes[Rid+1]->related_rules.size();k++)
						Nodes[j+1]->related_rules.insert(Nodes[j+1]->related_rules.end(),Nodes[Rid+1]->related_rules[k]);
					Nodes[j+1]->related_rules.insert(Nodes[j+1]->related_rules.end(),Rid+1);
					Nodes[Rid+1]->related_rules.insert(Nodes[Rid+1]->related_rules.end(),j+1);
					Nodes[Rid+1]->sonList.insert(Nodes[Rid+1]->sonList.end(),Nodes[j+1]);
					/*  for(uint32_t k=0;k<Nodes[Rid+1]->related_rules.size();k++)
					    printf("%u ",Nodes[Rid+1]->related_rules[k]);
					    printf("\n");                                    
					    for(uint32_t k=0;k<Nodes[j+1]->related_rules.size();k++)
					    printf("%u ",Nodes[j+1]->related_rules[k]);
					    printf("\n"); */

				}
				else
					printf("%d is reachable through %d \n",j+1,Rid+1);


			}

		}
	}
}


bool DepGraph::isReachable(uint32_t src,uint32_t dest)
{
	//				printf("src %u ,dest:%u \n",src,dest);
	uint32_t sz=Nodes[src]->related_rules.size();
	uint32_t i=0;
	if(sz==0)
		return false;
	else{

		for(i=0;i<sz;i++)
		{
			if(Nodes[src]->related_rules[i]==dest)
				return true;
		}

		return false;
	}



}

