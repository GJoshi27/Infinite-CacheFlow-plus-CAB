#ifndef RULELIST_H
#define RULELIST_H

#include "stdafx.h"
#include "Address.hpp"
#include "Rule.hpp"
#include <unordered_map>

class rule_list
{
public:
    std::vector<rule> list;
    std::unordered_map <uint32_t, std::vector<uint32_t> > dep_map;
    std::vector<size_t> occupancy;

    rule_list();
    rule_list(std::string &);
    void obtain_dep();
		void print_dep();
		void sort_list();
    void print(const std::string &);
};
#endif
