#ifndef _SoftwareSwitch_
#define _SoftwareSwitch_

#include "Rule.hpp"
#include "BucketSet.h"
#include "RuleList.h"

class SoftwareSwitch
{

	public:
		std::vector<Rule> entries;
		uint32_t CacheHit;

		inline SoftwareSwitch();
		inline void installRules(const Bucket &,const Rule_List &);

};


inline SoftwareSwitch::SoftwareSwitch()
{
	cout  <<  "Software Switch created \n";				
	entries=std::vector<Rule>();
	CacheHit=0;
}

inline void SoftwareSwitch::installRules(const Bucket &bkt,const Rule_List &rl)
{
	cout  <<  "Installing rules to Software Switch \n";

	uint32_t sz=bkt.NoRules;                                                
	uint32_t index=0;
	for(uint32_t i=0;i<sz;i++)                                              
	{                                                                       
		index=bkt.rules[i];                                             
		//cout   <<   "rule id "  <<   index;                                 
		entries.insert(entries.end(),rl.list[index-1]);                 
	}       
	return ;
}
#endif	
