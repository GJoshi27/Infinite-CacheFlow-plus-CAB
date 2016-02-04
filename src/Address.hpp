#ifndef ADDRESS_H
#define ADDRESS_H

#include "stdafx.h"
#include <boost/functional/hash.hpp>
class pref_addr {
  public:
    uint32_t pref;
    uint32_t mask;

  public:
    inline pref_addr();
    inline pref_addr(const pref_addr &);
    inline pref_addr(const std::string &);
    inline bool operator==(const pref_addr &) const;
    inline bool match (const pref_addr &) const;
    inline void print() const;
    inline std::string get_str() const;
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
    inline bool match (const pref_addr &) const;
    inline void print() const;
    inline std::string get_str() const;
};


// ---------------------- pref_addr ---------------------

inline pref_addr::pref_addr() {
    pref = 0;
    mask = 0;
}

inline pref_addr::pref_addr(const string & prefstr) {
    vector<string> temp1;
    boost::split(temp1, prefstr, boost::is_any_of("/"));

    uint32_t maskInt = boost::lexical_cast<uint32_t>(temp1[1]);
    //printf("Gaurav maskInt  is :%u \n",maskInt);
    mask = 0;
    if (maskInt != 0)
        mask = ((~uint32_t(0)) << (32-maskInt));
        /* ~ of zero is  all 1's and then left shifting it to 32-maskInt 
				 will give me what I want in binary mask */

    //printf(" Gaurav After doing the operation mask : %u \n",mask);

    vector<string> temp2;
    boost::split(temp2, temp1[0], boost::is_any_of("."));

    pref = 0;
    for(uint32_t i=0; i<4; i++) {
        pref = (pref<<8) + boost::lexical_cast<uint32_t>(temp2[i]); //8 bits left Shifting  for each one 
        //printf("Gaurav pref : %u \n",pref);
    }
    pref=(pref & mask);
    //printf("Gaurav After anding with mask pref: %u \n",pref);
}

inline pref_addr::pref_addr(const pref_addr & pa) {
    pref = pa.pref;
    mask = pa.mask;
}

/*  for removing duplicates */
inline bool pref_addr::operator==(const pref_addr & rhs) const{
	if (pref != rhs.pref)
		return false;
//	printf("1 in operator == true \n");
	if (mask != rhs.mask)
		return false;
//	printf("2 in operator == returing true\n");
	return true;
}


inline bool pref_addr::match(const pref_addr & ad) const {
    uint32_t mask_short;
    if (mask > ad.mask)
        mask_short = ad.mask;
    else
        mask_short = mask;

    return ((pref & mask_short) == (ad.pref & mask_short));
}

inline void pref_addr::print() const {
    cout<<get_str()<<endl;
}

inline string pref_addr::get_str() const {
    stringstream ss;
    for (uint32_t i = 0; i<4; i++) {
        ss<<((pref>>(24-(i*8))&((1<<8)-1)));
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
 * for hash_bashed and comparison based use
 */
inline bool range_addr::operator<(const range_addr & ra) const {
    return range[0]< ra.range[0];
}

inline bool range_addr::operator==(const range_addr & ra) const {
    return ( range[0] == ra.range[0] && range[1] == ra.range[1]);
}

inline uint32_t hash_value(range_addr const & ra) {
    size_t seed = 0;
    boost::hash_combine(seed, ra.range[0]);
    boost::hash_combine(seed, ra.range[1]);
    return seed;
}

/* member function                                                              
 *  */                                                                             
inline bool range_addr::overlap(const range_addr & ad) const { // whether two range_addr overlap  sym
				    return (!(range[1] < ad.range[0]) || (range[0] > ad.range[1]));             
}  

inline bool range_addr::match(const pref_addr & ad) const { // whether a range matchs a prefix  sym
    return (! ((range[1] & ad.mask) < ad.pref || (range[0] & ad.mask) > ad.pref));
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

#endif

