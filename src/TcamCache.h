#ifndef _TCAM_
#define _TCAM_

#include "Rule.hpp"
#include "BucketSet.h"
#include "RuleList.h"

class Cache
{

	public:
		std::vector<Rule> entries;
		uint32_t size;
		uint32_t AvailSpace;
		uint32_t CacheHit;
		uint32_t CacheMisses;
		uint32_t noOfBckts;


		Cache(uint32_t,uint32_t);
		int initCache(BucketSet &);
		void installRules(const Bucket &,const Rule_List &);

};
#endif	
