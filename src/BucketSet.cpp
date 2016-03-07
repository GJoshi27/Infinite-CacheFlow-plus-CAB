#include "DepGraph.h"
#include "BucketSet.h"
#include <boost/log/trivial.hpp>    

bool compare(const Bucket &,const Bucket &);

BucketSet::BucketSet(DepGraph &DGraph)
{
	BOOST_LOG_TRIVIAL(info)<<  "Constructor BucketSet Started";
	uint32_t sz=DGraph.root->sonList.size(),i,tsz=1;                        
	Node *tmp=new Node(100000);                                             
	NoOfBuckets=0;

	/*  Traversal of Dependency Tree for each immediate child of Root */       
	for(i=0;i<sz;i++)                                                       
	{                                                                       
		tmp=DGraph.root->sonList[i];                                    
		Bucket btmp;
		tsz=1;                                                          
		while(1){                                                       
			// cout  <<  tmp->rule_id  << "-> " ;                      
			btmp.rules.insert(btmp.rules.end(),tmp->rule_id);   
			tsz=tmp->sonList.size();                                
			if(tsz!=0)                                              
				tmp=tmp->sonList[0];                            
			else                                                    
				break;                                          
		}                                                               
		bckts.insert(bckts.end(),btmp);                                 
		NoOfBuckets++;
		//cout   <<  "NULL" <<  endl;                                     

	}                                                  
	BOOST_LOG_TRIVIAL(info) <<  "Constructor BucketSet finished ";

}

          
void BucketSet::print()
{
	BOOST_LOG_TRIVIAL(info)<<  "BucketSet::print() started" ;
	uint32_t sz=bckts.size();
	cout  <<  "Number of Buckets are "  << NoOfBuckets <<  endl;

	Bucket btmp;
	for(uint32_t i=0;i <sz ;i++)
	{
		cout  <<  "Bucket "  <<  i  <<  ": ";
		btmp=bckts[i];
		btmp.print();
		cout  <<  endl;
	}

	BOOST_LOG_TRIVIAL(info)  <<  "BucketSet::print() finished" ;

}

void BucketSet::sortBckts(uint32_t idx,char ind)
{
	if(ind=='S')
		std::sort(bckts.begin()+idx,bckts.end());
	else if(ind=='P') //Sorting based on priority
		std::sort(bckts.begin(),bckts.end(),compare);
}

void BucketSet::splitBuckets(uint32_t maxBuckets,uint32_t maxBktSz)
{
	uint32_t sz=bckts.size();
	BOOST_LOG_TRIVIAL(info)  <<  "BucketSet::splitBuckets() started ";
	cout  <<  "Number of Buckets are "  << sz <<  endl;
	cout  <<  "Maximum Bucket Size " <<  maxBktSz  << endl;

	Bucket btmp;
	for(uint32_t i=0;i <sz ;i++)
	{
		sz=bckts.size();
		if(sz==maxBuckets)
		{
			break;
		}
		if(bckts[i].rules.size()>maxBktSz)
		{
			std::vector<uint32_t> v2(bckts[i].rules.begin(), bckts[i].rules.begin() + maxBktSz);
			std::vector<uint32_t> v3(bckts[i].rules.begin() + maxBktSz, bckts[i].rules.end());
			bckts[i].rules=v2;
			btmp.rules=v3;
			bckts.insert(bckts.end(),btmp);
			sortBckts(i+1,'S');
			NoOfBuckets++;

		}

	}

	BOOST_LOG_TRIVIAL(info)  <<  "BucketSet::splitBuckets() finished";
	return;
}

int BucketSet::findMatchingBucket(const addr_5tup &pkt)                        
{                                                                               

	uint32_t sz=bckts.size();                                               

	for(uint32_t i=0;i <sz ;i++)                                            
	{                                                                       
		if (bckts[i].pattern.packet_hit(pkt))                           
			return i;                                             
	}                                                                       

	return -1; // No matching bucket found
}   

bool compare(const Bucket &lhs, const Bucket &rhs)               
{                                                                               
	return lhs.Priority > rhs.Priority;                                 
}

