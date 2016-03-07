#ifndef ADDRESS_H
#define ADDRESS_H

#include "stdafx.h"
class prefix_addr {
  public:
    uint32_t prefix;
    uint32_t mask;

  public:
    inline prefix_addr();
    inline prefix_addr(const prefix_addr &);
    inline prefix_addr(const std::string &);
    inline bool operator==(const prefix_addr &) const;
    inline bool match (const prefix_addr &) const;
    inline void print() const;
    inline std::string get_str() const;
		inline bool hit (const uint32_t &) const;
};


class range_addr {
  public:
    uint32_t range[2];

  public:
    inline range_addr();
    inline range_addr(const std::string &);
    inline bool operator<(const range_addr &) const;
    inline bool operator==(const range_addr &) const;
    inline bool overlap (const range_addr &) const;
    inline bool match (const prefix_addr &) const;
    inline void print() const;
		inline bool hit (const uint32_t &) const;
    inline std::string get_str() const;
};

class addr_5tup {                                                               
  public:                                                                       
    uint32_t addrs[5];                                                          
                                                                                
  public:                                                                       
    inline addr_5tup();                                                         
    inline addr_5tup(const addr_5tup &);                                        
    inline addr_5tup(const std::string &); // processing gen                    
		inline uint32_t conv_prefix(const std::string &);
};


using std::vector;
using std::string;
using std::stringstream;
using std::cout;
using std::endl;



// ---------------------- prefix_addr ---------------------

inline prefix_addr::prefix_addr() {
    prefix= 0;
    mask = 0;
}

inline prefix_addr::prefix_addr(const string & prefstr) {
    vector<string> temp1;
    boost::split(temp1, prefstr, boost::is_any_of("/"));

    uint32_t maskInt = boost::lexical_cast<uint32_t>(temp1[1]);
    //printf("maskInt  is :%u \n",maskInt);
    mask = 0;
    if (maskInt != 0)
        mask = ((~uint32_t(0)) << (32-maskInt));
        /* ~ of zero is  all 1's and then left shifting it to 32-maskInt 
				 will give me what I want in binary mask */

    //printf(" After doing the operation mask : %u \n",mask);

    vector<string> temp2;
    boost::split(temp2, temp1[0], boost::is_any_of("."));

    prefix = 0;
    for(uint32_t i=0; i<4; i++) {
        prefix = (prefix<<8) + boost::lexical_cast<uint32_t>(temp2[i]); //8 bits left Shifting  for each one 
        //printf("pref : %u \n",pref);
    }
    prefix=(prefix & mask);
    //printf("After anding with mask pref: %u \n",pref);
}

inline prefix_addr::prefix_addr(const prefix_addr & pa) {
    prefix = pa.prefix;
    mask = pa.mask;
}

/*  for removing duplicates */
inline bool prefix_addr::operator==(const prefix_addr & rhs) const{
	if (prefix != rhs.prefix)
		return false;
//	printf("1 in operator == true \n");
	if (mask != rhs.mask)
		return false;
//	printf("2 in operator == returing true\n");
	return true;
}


inline bool prefix_addr::match(const prefix_addr & ad) const {
    uint32_t mask_short;
    if (mask > ad.mask)
        mask_short = ad.mask;
    else
        mask_short = mask;

    return ((prefix & mask_short) == (ad.prefix & mask_short));
}

inline bool prefix_addr::hit(const uint32_t & ad) const {                         
				    return (prefix == (ad & mask));                                               
}  

inline void prefix_addr::print() const {
    cout<<get_str()<<endl;
}

inline string prefix_addr::get_str() const {
    stringstream ss;
    for (uint32_t i = 0; i<4; i++) {
        ss<<((prefix>>(24-(i*8))&((1<<8)-1)));
        if (i != 3)
            ss<<".";
    }
    ss<<"/";

    uint32_t m = 0;
    uint32_t mask_cp = mask;

    if ((~mask_cp) == 0) {
        ss<<32;
        return ss.str();
    }
    for (uint32_t i=0; mask_cp; i++) {
        m++;
        mask_cp = (mask_cp << 1);
    }
    ss<<m;
    return ss.str();
}


/* ---------------------- range_addr ---------------------
 * Port Numbers
 * range address: two icons are range start and termin
 */

inline range_addr::range_addr() {
    range[0] = 0;
    range[1] = 0;
}


inline range_addr::range_addr(const string & rangestr) {
   //printf("Gaurav Inside range_addr const string & rangestr \n");
    vector<string> temp1;
    boost::split(temp1, rangestr, boost::is_any_of(":"));
    boost::trim(temp1[0]);
    boost::trim(temp1[1]);
    range[0] = boost::lexical_cast<uint32_t> (temp1[0]);
    range[1] = boost::lexical_cast<uint32_t> (temp1[1]);
}
/* operator functions
 *
 * for comparison based use
 */
inline bool range_addr::operator<(const range_addr & ra) const {
    return range[0]< ra.range[0];
}

inline bool range_addr::operator==(const range_addr & ra) const {
    return ( range[0] == ra.range[0] && range[1] == ra.range[1]);
}


/* member function                                                              
 *  */                                                                             
inline bool range_addr::overlap(const range_addr & ad) const { 
				// whether two range_addr overlap 
			  return (!(range[1] < ad.range[0]) || (range[0] > ad.range[1]));             
}  

inline bool range_addr::match(const prefix_addr & ad) const { 
			// whether a range matchs a prefix  sym
    return (! ((range[1] & ad.mask) < ad.prefix || (range[0] & ad.mask) > ad.prefix));
}

inline bool range_addr::hit(const uint32_t & ad) const { // whether a packet hit
				    return (range[0] <= ad && range[1] >= ad);                                  
} 


/* print function
 */
inline void range_addr::print() const {
    cout<< get_str() <<endl;
}

inline string range_addr::get_str() const {
    stringstream ss;
    ss<<range[0] << ":" << range[1];
    return ss.str();
}


/*  Methods of addr_tuple */

inline addr_5tup::addr_5tup(){}

inline addr_5tup::addr_5tup(const string & rule_str){                                
				vector<string> temp;                       
				//cout  <<  "Inside addr_5tup\n";
				//try{
				boost::split(temp, rule_str, boost::is_any_of("\t"));
				addrs[0] = conv_prefix(temp[0]);
				addrs[1] = conv_prefix(temp[1]);
				addrs[2] = boost::lexical_cast<uint32_t>(temp[2]);                          
				addrs[3] = boost::lexical_cast<uint32_t>(temp[3]);                          

				addrs[4]=  0; //Chang accordingly for protocol if required
			/*	}catch(const std::exception& ex)
				{
								throw "Invalid Input";
				} */


}

inline uint32_t addr_5tup::conv_prefix(const string &ip)
{
				//cout  <<  "Inside conv_prefix\n";
				vector<string> temp1;                                                       
				boost::split(temp1,ip, boost::is_any_of("."));  
				uint32_t prefix=0;                                                      
				for(uint32_t i=0; i<4; i++) {                                               
								prefix = (prefix  <<  8) + boost::lexical_cast<uint32_t>(temp1[i]); //8 bits left Shifting  for each one 
				}

				return prefix;

}
#endif

