#include <iostream>
#include <string>
#include "TcamCache.h"
#include "SoftwareSwitch.hpp"
#include <boost/log/trivial.hpp>

using namespace std; 

int main(int argc, char* argv[])
{
	if(argc < 7) 
	{
		cerr << "Usage: CacheCode {rules_file} {TCAM_Size} "
			<<"{% of TCAM allowed for Buckets} {Lower_Threshold}"
			<<  "{Upper Threshold} {FilehavingPcktstuples}"
			<< endl;
		exit(1);
	} 
	/*  Parsing Input Parameters  */
	BOOST_LOG_TRIVIAL(info)  <<  "---CacheFlow Started ---  ";

	uint32_t maxBuckets=0,maxBktSz=0;
	int found=0;
	uint32_t TcamSz= boost::lexical_cast<uint32_t>(argv[2]); 
	uint32_t BucketSpace= boost::lexical_cast<uint32_t>(argv[3]); 
	uint32_t LowerThreshold_Rc= boost::lexical_cast<uint32_t>(argv[4]); 
	uint32_t UpperThreshold_Rc= boost::lexical_cast<uint32_t>(argv[5]); 

	maxBuckets=floor((BucketSpace*TcamSz)/100);
	maxBktSz=(TcamSz-maxBuckets)/maxBuckets;
	ifstream pcktfile;
	pcktfile.open(argv[6]);
	if(!pcktfile)
	{
		BOOST_LOG_TRIVIAL(fatal)   <<  "Error: Unable to open file "   
			<< pcktfile << std::endl;
		exit(2);
	}


	/*  rList  */
	string rulefile(argv[1]);
	Rule_List rList(rulefile); /* Saving rules into List  rList */
	//rList.print("output.txt");
	printf("Obtaining dep list");
	rList.sort_list();
	printf("printing sorted list to a file SortedRuleList.txt \n");
	rList.print("SortedRuleList.txt");
	rList.obtain_dep();
	rList.print_dep();

	/*  Tree construction */

	DepGraph Dgraph(rList);
	BOOST_LOG_TRIVIAL(info)  <<  "Max Buckets allowed are : " 
		<< maxBuckets  <<  endl;

	if(Dgraph.root->sonList.size() > maxBuckets)
	{
		BOOST_LOG_TRIVIAL(error) <<  "Cannot proceed as number of child nodes of root "
			<<  "exceeds maxBuckets"  <<  endl;
		exit(3);
	}

	/*  Initialising Bucket Set */
	BucketSet BktSet(Dgraph);
	BktSet.print();
	BktSet.sortBckts(0,'S'); //Sorting Bucket based on size of Buckets

	BOOST_LOG_TRIVIAL(info) <<  "After sorting Buckets" <<  endl;
	BktSet.print();

	/*  Splitting BucketSet if initial Number of Buckets
	 * is less than maxBuckets
	 * If Initial Number of Buckets is equal to maxBuckets 
	 * splitting of Buckets is not required*/
	if(BktSet.bckts.size()< maxBuckets)
	{
		BktSet.splitBuckets(maxBuckets,maxBktSz);
		BOOST_LOG_TRIVIAL(info) <<  "after splitting "  << endl;
		//BktSet.print();
	}

	for(uint32_t i=0;i<BktSet.bckts.size();i++)
	{
		BktSet.bckts[i].eValOther(rList.list);

	}

	BktSet.print();

	/* Sorting Bucket based on priority of Buckets
	 * as we want to search for a matching Bucket
	 * which is of highest priority */
	BktSet.sortBckts(0,'P'); 
	BktSet.print();


	/* Initialisation of Cache by Controller.
	 * Proactive Installation of Buckets in TCAM */


	Cache Tcam(TcamSz,BktSet.NoOfBuckets); /* Creating TCAM Cache object */
	Tcam.initCache(BktSet);          /* Cache initialised with Buckets */

	SoftwareSwitch SftSw;

	/* Parsing input file for rules */

	BOOST_LOG_TRIVIAL(info) << "Waiting for packets " << endl;

	string sLine = "";
	getline(pcktfile, sLine);
	while (!pcktfile.eof())
	{
		found=-1;
		BOOST_LOG_TRIVIAL(info) << "packets " << endl;
		addr_5tup *pckt=NULL; // Converting Line from packet file
		try{
			pckt=new addr_5tup(sLine); // Converting Line from packet file

			/*Finding highest priority matching Bucket for the packet */

			found=BktSet.findMatchingBucket(*pckt);
		}catch( const std::exception& ex)                                        
		{                                                                       
			BOOST_LOG_TRIVIAL(error) <<   "Error while parsing packets\n" ;
			BOOST_LOG_TRIVIAL(error) <<   ex.what() ;               
			BOOST_LOG_TRIVIAL(info) <<  "\nSkipping this pckt \n";
			getline(pcktfile, sLine);
			continue;
		}          
		if(found==-1)
			BOOST_LOG_TRIVIAL(info) <<  "No matching Bucket found for this packet\n";
		else
		{
			BOOST_LOG_TRIVIAL(info) << "Matching Bucket found: " << found  << endl;
			Bucket matchedbkt = BktSet.bckts[found];
			char Installed=matchedbkt.installInd;
			BOOST_LOG_TRIVIAL(info) <<  "installed " <<  Installed  <<  endl;
			if(Installed=='N')
			{
				/*  MAtching Bucket is not installed in TCAM
				 *  or Software Switch.
				 *  Install Bucket in either TCAM/SS ,then 
				 *  after that perform packet matching*/
				char place=matchedbkt.findWheretoInstall
					(LowerThreshold_Rc,Tcam.AvailSpace);

				if(place=='S')
				{
					/*Install rules to Software Switch*/
					SftSw.installRules(matchedbkt,rList);
					BktSet.bckts[found].installInd='S';
				}
				else
				{
					/* Install rules to TCAM */
					Tcam.installRules(matchedbkt,rList);
					BktSet.bckts[found].installInd='T';
				}

			}
			Installed=BktSet.bckts[found].installInd;

			/*  Now performing packet matching */
			int ruleid=matchedbkt.findMatchingRule(*pckt,rList.list);
			BOOST_LOG_TRIVIAL(info) <<  "Rule matched: " <<  ruleid  <<  endl;

		}

		getline(pcktfile, sLine);
	}

	/*  close file descriptors */
	pcktfile.close();

	BOOST_LOG_TRIVIAL(info)  <<  "--- End of CacheFlow ---";
	exit(0);    
}

