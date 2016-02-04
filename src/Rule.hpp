#ifndef RULE_H
#define RULE_H

#include "stdafx.h"
#include "Address.hpp"
#include <boost/functional/hash.hpp>

class rule {
  public:
    pref_addr hostpair[2];
    range_addr portpair[2];
    bool proto;
		uint32_t weight;
		uint32_t priority;

    bool hit;

  public:
    inline rule();
    inline rule(const std::string &);
    inline bool operator==(const rule &) const;
		 inline bool dep_rule(const rule &) const; 
    inline std::string get_str() const;
    inline void print() const;
};

/*  For sorting based on priority */                                                             
struct less_than_key                                                            
{                                                                               
				inline bool operator() (const rule& lhs, const rule& rhs)           
				{                                                                       
								return (lhs.priority < rhs.priority);                           
				}                                                                       
};  

using std::pair;
using std::endl;

/* ----------------------------- rule----------------------------------
 * brief:
 * bucket_rules: two dim are prefix address, the other two are range address
 */

/* constructors
 *
 * options:
 * 	()			default
 * 	(const rule &)	copy function
 * 	(const string &)	generate from a string "#srcpref/mask \t dstpref/mask \t ..."
 */

inline rule::rule(const string & rule_str) {                 
    vector<string> temp;                                                        
    boost::split(temp, rule_str, boost::is_any_of("\t")); //spliting tab separated values
    //printf("Inside rule \n");                                               
    temp[0].erase(0,1); //Why this is there Remove 0 till 1st not 1st. I think because 0 position as @
    hostpair[0] = pref_addr(temp[0]);  // For changing ip and mask into a single entity using AND operation
    hostpair[1] = pref_addr(temp[1]);                                           
    portpair[0] = range_addr(temp[2]); // For spliting portnumbers separated by : range!!Open tag 5
    portpair[1] = range_addr(temp[3]);                                      
    boost::trim(temp[6]);                                                       
    //cout  <<  temp[6]  <<  endl;                                                
    priority= boost::lexical_cast<uint32_t>(temp[6]);                           
		//printf("priority : %ld \n",priority);
    proto = true;                                                               
    hit = false;                                                                
}     

inline bool rule::operator==(const rule & rhs) const {
    if (!(hostpair[0] == rhs.hostpair[0]))
        return false;
    if (!(hostpair[1] == rhs.hostpair[1]))
        return false;
    if (!(portpair[0] == rhs.portpair[0]))
        return false;
    if (!(portpair[1] == rhs.portpair[1]))
        return false;
    return true;
}

/* member fuctions
 */

inline bool rule::dep_rule(const rule & rl) const { // check whether a rule is directly dependent
				if (!hostpair[0].match(rl.hostpair[0])){                                    
								//  printf("1 hostpair not matched\n");                               
								return false;                                                           
				}                                                                           
				if (!hostpair[1].match(rl.hostpair[1])){                                    

								//  printf("2 hostpair not matched\n");                               
								return false;                                                           
				}                                                                           
				if (!portpair[0].overlap(rl.portpair[0]))                                   
				{                                                                           
								//  printf("1 portpair not matched\n");                               
								return false;                                                           
				}                                                                           
				if (!portpair[1].overlap(rl.portpair[1])){                                  
								//    printf("2 portpair not matched\n");                               
								return false;                                                           
				}                                                                           
				printf("all matched its dependent \n");                                     
				return true;                                                                
}                                                                               

/* debug & print function
 */

inline void rule::print() const {
    cout<<get_str()<<endl;
}

inline string rule::get_str() const {
    stringstream ss;
    ss<<hostpair[0].get_str()<<"\t";
    ss<<hostpair[1].get_str()<<"\t";
    ss<<portpair[0].get_str()<<"\t";
    ss<<portpair[1].get_str()<<"\t";
    if (proto)
        ss<<"tcp" << "\t";
    else
        ss<<"udp"  <<  "\t";
		//std::ostringstream ostr; //output string stream
	//	ostr  <<  priority;
	//	std::string theNumberString = ostr.str();
	//	cout  <<  theNumberString;
	//	ss  <<  theNumberString;
//	printf("priority : %u\n",priority);
	ss <<  priority;
    return ss.str();
}
#endif




