#ifndef _BUCKET_H
#define _BUCKET_H

#include "stdafx.h"
#include "Rule.hpp"
#include "RuleList.h"
#include <boost/log/trivial.hpp>    

class Bucket{
  public:
		rule pattern;
    std::vector<uint32_t> rules;// IDs of rules in the Bucket
    uint32_t TotWeight;
    uint32_t NoRules;
    uint32_t Priority;
		char installInd;

  public:
    inline Bucket();
    inline void print();
		inline bool operator<(const Bucket &) const;
		inline void eValOther(const std::vector<rule> &);
		inline char findWheretoInstall(uint32_t,uint32_t);
		inline int findMatchingRule(const addr_5tup &,const std::vector<rule> &);
};

inline Bucket::Bucket()
{
				rules=std::vector<uint32_t>();
				TotWeight=0;
				Priority=0;
				NoRules=0;
				installInd='N';
}

inline void Bucket::print()
{
				for(uint32_t i=0;i<rules.size();i++)
				{
								std::cout  <<  rules[i]  << " ";
				}
				std::cout  <<  "Weight: " << TotWeight;
				std::cout  <<  " Priority: " << Priority;
				std::cout  <<  " NoRules: " << NoRules;

}
inline bool Bucket::operator<(const Bucket &rhs) const{        
			return (rules.size() > rhs.rules.size());                               
}

inline void Bucket::eValOther(const std::vector<rule> &rl)
{
				uint32_t sz=rules.size();
				NoRules=sz;
				uint32_t idx=0;
				for(uint32_t i=0;i<sz;i++)
				{
								idx=rules[i]-1;
								if(i==sz-1){
												Priority=rl[idx].priority;
												pattern=rl[idx];
								}
								TotWeight=TotWeight+rl[idx].weight;
								installInd='N';
				}

				return ;

}

inline char Bucket::findWheretoInstall(uint32_t L_thres,uint32_t AvailSpace)
{
				BOOST_LOG_TRIVIAL(info)  <<  "Function: Bucket::findWheretoInstall";
				uint32_t curr_Rc=floor(TotWeight/NoRules);
				cout   <<   "Current R_c ratio : "<<  curr_Rc  <<   endl;
				char ans;
				if(curr_Rc<=L_thres || AvailSpace < NoRules)                          
				{                                               
								/*Install rules to software switch    */
								ans='S';
				}                                               
				else 
				{                                          
								/*  Install rule to TCAM */             
								ans='T';

				}   

				BOOST_LOG_TRIVIAL(info)  <<  "Function: Bucket::findWheretoInstall Finished";
				return ans;

}

inline int Bucket::findMatchingRule(const addr_5tup &pckt,
								const std::vector<rule> &rl)
{

				BOOST_LOG_TRIVIAL(info)  <<  "Function: Bucket::findMatchingRule Started";
				cout  <<  "Matching most specific rule  \n";
				int idx=0;

				for(uint32_t i=0;i<NoRules;i++)
				{
								idx=rules[i];
								//cout  <<  "idx "  <<  idx;
								if(rl[idx-1].packet_hit(pckt)){
												rl[idx-1].print();
												return idx;
								}

				}

				BOOST_LOG_TRIVIAL(info)  <<  "Function: Bucket::findMatchingRule Finished";
				return -1;
}
#endif

