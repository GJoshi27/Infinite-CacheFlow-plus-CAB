#include "RuleList.h"

using std::ifstream;
using std::ofstream;
using std::string;

/* constructor
 *
 * options:
 * 	()			default
 * 	(string &)		draw from file
 */
Rule_List::Rule_List() {}

Rule_List::Rule_List(string & filename){
    ifstream file;
    file.open(filename.c_str());
		if(!file){
						std::cerr  <<  "Error: Unable to open file "   << filename   << std::endl;
		exit(-1);
		}
    printf("Making Rule_List \n");
    string sLine = "";
    getline(file, sLine);
    while (!file.eof()) {
				//cout  <<  "Inside file check "  <<  endl;		
				try{
        Rule sRule(sLine); // Converting Line from rules file into a structure sRule
        sRule.print();          //Change Gaurav Printing rule stored 
        list.push_back(sRule); //list variable of Rule_List vector type
				}catch( const std::exception& ex)
				{

								cout    <<    "Something went wrong while parsing\n"   <<    ex.what() ;       
								cout  <<  ex.what();
								cout   <<   "\nSkipping this rule\n"; 
								getline(file, sLine); 
								continue;
				}
        getline(file, sLine);
    }
    file.close();

    printf("Gaurav List size: %u \n",list.size());

    /*Below removing redudant rules from the list if any from the file */
  
    for(auto iter = list.begin(); iter != list.end(); ++iter)
    {
        for (auto iter_cp = iter+1; iter_cp != list.end(); ) {
            if (*iter == *iter_cp) {
                iter_cp = list.erase(iter_cp);
            }  
	else
		++iter_cp;
        }
    } 

    printf("Gaurav after remvoing redundancy List size: %u \n",list.size());
}

/* member func
 */

void Rule_List::obtain_dep() { // obtain the dependency map                     
    for(uint32_t idx = 0; idx < list.size(); ++idx) {                           
        vector <uint32_t> dep_rules;                                            
        //printf("\nouter loop\n");                                             
//        list[idx].print();                                                    
        for (uint32_t idx1 = idx+1; idx1 < list.size(); ++idx1) { //changing for now it should be <= and idx1 should start from idx
        //printf("\nInside loop\n");                                            
  //      list[idx1].print();                                                   
            if (list[idx].dep_rule(list[idx1])) {                               
                dep_rules.push_back(idx1);                                      
            }                                                                   
        }                                                                       
        dep_map[idx] = dep_rules;                                               
    }                                                                           
}      

/*
 * debug and print
 */
void Rule_List::print(const string & filename) {
    ofstream file;
    file.open(filename.c_str());
    for (vector<Rule>::iterator iter = list.begin(); iter != list.end(); iter++) {
				cout  <<  iter->get_str()  <<  endl;		
        file<<iter->get_str()<<endl;
    }
    file.close();
}


/*  Function for printing Dependency map matrix */                              
void Rule_List::print_dep() {                                                   
        cout   <<   "Printing Dependency matrix "   <<  endl;                   
        for ( uint32_t i = 0; i < dep_map.size(); i++ )                         
        {       
								std::cout  <<  i+1  << ": ";
                for ( uint32_t  j= 0; j < dep_map[i].size(); j++ )              
                {                                                                   
                        std::cout  <<  dep_map[i][j]+1  <<  ' ';                  
                }                                                                   
                std::cout  <<  std::endl;                                           
        }                                                                       
}
/*  Method for Sorting Rule List based on priority */
void Rule_List::sort_list(){
				printf("Inside sort list function \n");
				//std::sort(list.begin(),list.end(),less_than_key());
				std::sort(list.begin(),list.end());
				printf("Sorting done \n");
}



