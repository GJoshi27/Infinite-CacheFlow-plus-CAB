#ifndef _TCAM_
#define _TCAM_

#include "Rule.hpp"
#include "BucketSet.h"
#include "RuleList.h"

class Cache
{

	public:
		std::vector<rule> entries;
		uint32_t CacheHit;
		uint32_t CacheMisses;
		uint32_t noOfBckts;


		Cache(uint32_t,uint32_t);
		int initCache(BucketSet &);

};
#endif	
