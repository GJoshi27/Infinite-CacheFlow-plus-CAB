#ifndef RULE_H
#define RULE_H

#include "stdafx.h"
#include "Address.hpp"

class Rule {
	public:
		prefix_addr hostpair[2];
		range_addr portpair[2];
		std::vector<uint32_t> protocol;
		uint32_t weight;
		uint32_t priority;
		uint32_t FwdAction;
		bool hit;

	public:
		inline Rule();
		inline Rule(const Rule &);
		inline Rule(const std::string &);
		inline bool operator==(const Rule &) const;
		inline bool operator<(const Rule &) const;
		inline bool dep_rule(const Rule &) const; 
		inline bool packet_hit(const addr_5tup &) const; 
		inline std::string get_str() const;
		inline void print() const;
};

/*  For sorting based on priority */                                                             
/*  commenting below instead of this used operator operloading for < operator */
/*  
struct less_than_key                                                            
{                                                                               
				inline bool operator() (const Rule& lhs, const Rule& rhs)           
				{                                                                       
								return (lhs.priority < rhs.priority);                           
				}                                                                       
};  */

using std::pair;
using std::endl;

/* ----------------------------- Rule----------------------------------
 * brief:
 * bucket_Rules: two dim are prefix address, the other two are range address
 */

/* constructors
 *
 * options:
 * 	()			default
 * 	(const Rule &)	copy function
 * 	(const string &)	generate from a string "#srcpref/mask \t dstpref/mask \t ..."
 */
inline Rule::Rule(){} 

inline Rule::Rule(const Rule & ru) {                                      
    hostpair[0] = ru.hostpair[0];                                               
    hostpair[1] = ru.hostpair[1];                                               
    portpair[0] = ru.portpair[0];                                               
    portpair[1] = ru.portpair[1];                                               
    protocol= ru.protocol;                                                      
    weight=ru.weight;                                                            
    priority=ru.priority;                                                          
    FwdAction=ru.FwdAction; 
    hit = ru.hit;                                                               
}    

inline Rule::Rule(const string & Rule_str) {                 
    vector<string> temp;                                                        
    boost::split(temp, Rule_str, boost::is_any_of("\t")); //spliting tab separated values
    //printf("Inside Rule \n");                                               
    temp[0].erase(0,1); //Why this is there Remove 0 till 1st not 1st. I think because 0 position as @
    hostpair[0] = prefix_addr(temp[0]);  // For changing ip and mask into a single entity using AND operation
    hostpair[1] = prefix_addr(temp[1]);                                           
    portpair[0] = range_addr(temp[2]); // For spliting portnumbers separated by : range!!Open tag 5
    portpair[1] = range_addr(temp[3]);                                      
    // temp[4] is protocol
    boost::trim(temp[6]);                                                       
    boost::trim(temp[7]);                                                       
    //boost::trim(temp[8]);                                                       
    priority= boost::lexical_cast<uint32_t>(temp[6]);                           
    weight= boost::lexical_cast<uint32_t>(temp[7]);                           
    //FwdAction= boost::lexical_cast<uint32_t>(temp[8]);                           
    hit = false;                                                                
}     

inline bool Rule::operator==(const Rule & rhs) const {
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
inline bool Rule::operator<(const Rule &rhs) const{
								return (priority < rhs.priority);                           
}

/* member fuctions
 */

inline bool Rule::dep_rule(const Rule & rl) const { // check whether a Rule is directly dependent
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
				//printf("all matched its dependent \n");                                     
				return true;                                                                
}                                                                               

/* debug & print function
 */

inline void Rule::print() const {
    cout<<get_str()<<endl;
}

inline string Rule::get_str() const {
    stringstream ss;
    ss<<hostpair[0].get_str()<<"\t";
    ss<<hostpair[1].get_str()<<"\t";
    ss<<portpair[0].get_str()<<"\t";
    ss<<portpair[1].get_str()<<"\t";
    /*  if (proto)
        ss<<"tcp" << "\t";
    else
        ss<<"udp"  <<  "\t"; */
		//std::ostringstream ostr; //output string stream
	//	ostr  <<  priority;
	//	std::string theNumberString = ostr.str();
	//	cout  <<  theNumberString;
	//	ss  <<  theNumberString;
//	printf("priority : %u\n",priority);
	ss <<  priority  <<  "\t";
	ss <<  weight;
    return ss.str();
}

inline bool Rule::packet_hit(const addr_5tup & packet) const {
 // check whehter a Rule is hit.
    if (!hostpair[0].hit(packet.addrs[0]))                                      
        return false;                                                           
    if (!hostpair[1].hit(packet.addrs[1]))                                      
        return false;                                                           
    if (!portpair[0].hit(packet.addrs[2]))                                      
        return false;                                                           
    if (!portpair[1].hit(packet.addrs[3]))                                      
        return false;                                                           
    // ignore protocol for now
    return true;                                                                
}   
#endif




