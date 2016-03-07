#ifndef _BUCKETSET_
#define _BUCKETSET_

#include "stdafx.h"
#include <set>
#include "Bucket.hpp"
#include "Address.hpp"
#include "DepGraph.h"

class BucketSet{
  public:
    std::vector<Bucket> bckts;// IDs of rules in the Bucket
    uint32_t NoOfBuckets;

  public:
    BucketSet(DepGraph &);
		void print();
		void sortBckts(uint32_t,char);
		void splitBuckets(uint32_t,uint32_t);
		int findMatchingBucket(const addr_5tup &);
};
#endif

