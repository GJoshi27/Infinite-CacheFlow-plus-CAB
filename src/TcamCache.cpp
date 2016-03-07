#include "Rule.hpp"
#include "RuleList.h"
#include "Bucket.hpp"
#include "TcamCache.h"

// --------- Bucket ------------

using namespace std;
#include<vector>

Cache::Cache(uint32_t sz,uint32_t bkts)
{
	entries=vector<rule>(sz);
	size=sz;
	AvailSpace=sz;
	CacheHit=0;
	CacheMisses=0;
	noOfBckts=bkts;

}

int Cache::initCache(BucketSet &bs)
{

	cout << "Intialise Cache with Buckets " << endl;
	uint32_t totalbkts=bs.NoOfBuckets;

	for(uint32_t i=0;i<totalbkts;i++)
	{
		cout << "Bucket: " << i << endl;
		rule tmp(bs.bckts[i].pattern);
		entries[i]=tmp;
		entries[i].print();
		bs.bckts[i].installInd='N';
	}

	return 0;

}

void Cache::installRules(const Bucket &bkt,const rule_list &rl)
{
				cout  <<  "Installing rule in TCAM \n";
				uint32_t sz=bkt.NoRules;
				uint32_t index=0;
				for(uint32_t i=0;i<sz;i++)
				{
								index=bkt.rules[i];
								cout  <<  "rule id " <<  index;
								entries.insert(entries.end(),rl.list[index-1]);
				}
				AvailSpace=size-sz;
				return;
}

