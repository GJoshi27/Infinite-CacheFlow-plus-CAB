#ifndef RULELIST_H
#define RULELIST_H

#include "stdafx.h"
#include "Address.hpp"
#include "Rule.hpp"
#include <unordered_map>

class Rule_List
{
	public:
		std::vector<Rule> list;
		std::unordered_map <uint32_t, std::vector<uint32_t> > dep_map;

		Rule_List();
		Rule_List(std::string &);
		void obtain_dep();
		void print_dep();
		void sort_list();
		void print(const std::string &);
};
#endif
