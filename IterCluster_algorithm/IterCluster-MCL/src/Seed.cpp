#include "Seed.h"
#include "SeedControl.h"
#include "Timer.h"
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <sys/time.h>
#include <unordered_map>
#include <pthread.h>
#include <stdlib.h>
#include <mutex>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>
#include <Eigen/Dense>
#include <Eigen/Core>
#include <Eigen/Eigen>

Seed::Seed(const unordered_map<int, vector<int > >& relate_seed_hash, const vector<vector<int> >& bar_unikmer_set,const unordered_map<int, int>& barcode_index)
{
    this->seed_barcode = 0;
    relate_hash_p = &relate_seed_hash;
    bar_unikmer_p = &bar_unikmer_set;
    barcode_index_p = &barcode_index;
}

void Seed::set_seed(int barcode)
{
    this->seed_barcode = barcode;
	this->seed_list.insert(make_pair(barcode, 1));
    if(barcode_index_p->count(barcode) > 0)
    {
       
    }else
    {
        cerr << "Error:seed barcode invalid" << "\n";
    }
}

void Seed::PrintSeedList(ofstream& output)
{
    for(unordered_map<int,int>::iterator iter = this->seed_list.begin(); iter != this->seed_list.end(); iter++)
    {
        output<< iter->first << "\t";
    }
    output << "\n";
}

int Seed::GetSeedBarcode()
{
    return this->seed_barcode;
}

vector<unordered_map<int, int> >* Seed::GetSubClusterList()

{
    return &(this->subCluster);
}

void Seed::addToSeedList(int barcode)
{
	this->seed_list.insert(make_pair(barcode, 1));
}

void Seed::clear()
{
	this->seed_list.clear();
	this->subCluster.clear();
}

bool Seed::clustering(ofstream& temOut)
{
	Timer t("clustering");
	//1.	start to preproceing
	unordered_map<int, int> opt_list;
	unordered_map<int, int> pre_unikmer_freq;
	for(unordered_map<int,int>::const_iterator iterS = this ->seed_list.begin(); iterS != this ->seed_list.end(); iterS++)
    {
    	//bulid pre unique kmer freq
    	if(barcode_index_p->count(iterS->first) > 0)
    	{
        	const int index = (barcode_index_p->find(iterS->first))->second;
        	vector<vector<int> >::const_iterator iterB = bar_unikmer_p->begin();
        	for(vector<int>::const_iterator iterK = (iterB + index)->begin(); iterK != (iterB + index)->end(); iterK++)
        	{
            	pre_unikmer_freq.insert(make_pair(*iterK, 1));
        	}
    	}else
    	{
        	cerr << "Error:seed barcode invalid" << "\n";
			return false;
    	}

		//get opt barcode
        if((this->relate_hash_p)->count(iterS->first) > 0 )
        {
            unordered_map<int, vector<int> >::const_iterator iter2 = (this->relate_hash_p)->find(iterS->first);
            for(vector<int>::const_iterator iter3 = iter2 -> second.begin(); iter3 != iter2 -> second.end(); iter3++)
            {
                if(this->seed_list.find(*iter3) == this->seed_list.end() && opt_list.find(*iter3) == opt_list.end())
                {
                    opt_list.insert(std::make_pair(*iter3, 1));
                }
            }
        }else
        {
            cerr << "In GetOptionalBarcode step: cannot find " << iterS->first << " in relate_hash" << endl;
			return false;
        }
    }
	unordered_map<int, int> pre_assistant_list;
	for(unordered_map<int,int>::iterator iterO = opt_list.begin(); iterO != opt_list.end(); iterO++)
	{
		int co_unikmer = 0;
		const int index = (barcode_index_p->find(iterO->first)) ->second;
		for(vector<int>::const_iterator iter2 = (bar_unikmer_p->begin()+index)->begin(); iter2 != (bar_unikmer_p->begin()+index)->end(); iter2++)
		{
			//double minUnikmerNum = std::min(barcode_info_map.at(iter->first).unikmerNum, (double)unikmer_freq.size());
			if(pre_unikmer_freq.find(*iter2) != pre_unikmer_freq.end())
			{
				co_unikmer ++;
			}
			if(co_unikmer  >= (int)cutoff1)
			{
				pre_assistant_list.insert(make_pair(iterO->first, 1));
				
				break;
			}
		}
	}
	temOut << "pre_assistant_list: " << pre_assistant_list.size() <<endl;

	//2.	start to process MCL
	//ofstream output("subCluster.txt");
	//assert(output.is_open());
	if(pre_assistant_list.size() > 5000)
	{
		cerr << "opt_list too big before MCL " << endl;
		return false;
	}
	const int dim = pre_assistant_list.size();
	Eigen::MatrixXf m = Eigen::MatrixXf::Zero(dim,dim);
	unordered_map<int,int> barcodeIndex;
	unordered_map<int,int> barcodeIndexReverse;
	int index = 0;
	for(unordered_map<int,int>::iterator iter2 = pre_assistant_list.begin(); iter2 != pre_assistant_list.end(); iter2++)
	{
		barcodeIndex.insert(make_pair(iter2->first, index));
		barcodeIndexReverse.insert(make_pair(index, iter2->first));
		index ++;
		if(index == dim)
		{
				break;
		}
	}
	temOut << "bulid index finish!" << endl;
					
	for(unordered_map<int,int>::iterator iter3 = pre_assistant_list.begin(); iter3 != pre_assistant_list.end(); iter3++)
	{
		if(barcodeIndex.find(iter3->first) != barcodeIndex.end())
		{
			for(vector<int>::iterator iter4 = relate_seed_hash.at(iter3->first).begin(); iter4 != relate_seed_hash.at(iter3->first).end(); iter4 ++)
			{
				if(barcodeIndex.find(*iter4) != barcodeIndex.end())
				{
					m(barcodeIndex.at(iter3->first), barcodeIndex.at(*iter4)) = 1;
				}
			}
		}
	}
	temOut << "bulid matrix finish!" << endl;
					
	for(int i = 0; i != iterTime; i++)
	{
		m = m * m;
		m = m.cwiseProduct(m);
		for(int j = 0; j != dim; j++)
		{
			double sum = 0;
			for(int k = 0; k != dim; k++)
			{
				sum += m(k,j);
			}
			for(int l = 0; l != dim; l++)
			{
				m(l,j) = m(l,j) / sum;
			}
		}
	}
	//temOut << "Matrix :" << "\n" << m << "\n";
					
	//start to mark down cluster
	//temOut << "start to mark down cluster" << endl;
	for(int s = 0; s != dim; s++)
	{
		unordered_map<int,int> cluster;
		for(int n = 0; n != dim; n++)
		{
			if(m(s,n) >= 0.1)
			{
				if(barcodeIndexReverse.find(n) != barcodeIndexReverse.end())
				{
					cluster.insert(make_pair(barcodeIndexReverse.at(n),1));
					barcodeIndexReverse.erase(n);
				}
			}
		}
		if(!cluster.empty() && cluster.size() > 50)
		{
			this->subCluster.push_back(cluster);
		}
	
	}
	//output sub cluster
	for(vector<unordered_map<int, int> >::iterator iterCC = subCluster.begin(); iterCC != subCluster.end(); iterCC++)
	{
		//temOut << "output sub cluster" << endl;
		for(unordered_map<int, int>::iterator iter = iterCC->begin(); iter != iterCC->end(); iter++)
		{
			//temOut << iter->first << "\t";
		}
		//temOut << endl;
	}
		/*
		for(vector<unordered_map<int, int> >::iterator iter = MCLcluster.begin(); iter != MCLcluster.end(); iter++)
		{
			for(unordered_map<int,int>::iterator iter2 = iter->begin(); iter2 != iter->end(); iter2++)
			{
				output << iter2->first << "\t";
			}
			output << endl;
		}
		*/
					
		//count the relation between cluster
		/*
		unordered_map<int,int> cluster1 = MCLcluster[0];
		unordered_map<int,int> cluster2 = MCLcluster[1];
		int coEdge = 0;
		for(unordered_map<int,int>::iterator iter3 = cluster1.begin(); iter3 != cluster1.end(); iter3++)
		{
			for(vector<int>::iterator iter4 = relate_seed_hash.at(iter3->first).begin(); iter4 != relate_seed_hash.at(iter3->first).end(); iter4++)
			{
				if(cluster2.find(*iter4) != cluster2.end())
				{
					coEdge ++;
				}
			}
		}
		cout << "coEdge: " << coEdge << endl;
		cout << "cluster1_size: " << cluster1.size() << endl;
		cout << "cluster2_size: " << cluster2.size() << endl;
		*/
	if(debug)
	{
		temOut << "start to print seed_list" << endl;
		for(unordered_map<int, int>::iterator iter = seed_list.begin(); iter != seed_list.end(); iter++)
		{
			temOut << iter->first << "\t";
		}
		temOut << endl;
	}
	//3.	start to enrich seed use IterCluster
	for(vector<unordered_map<int, int> >::iterator iterC = subCluster.begin(); iterC != subCluster.end(); iterC++)
	{
		unordered_map<int, int> subOpt_list;
		unordered_map<int, int> Tbarcode_list = *iterC;
		//bulid unique kmer freq 
		unordered_map<int, int> unikmer_freq;
		for(unordered_map<int, int>::iterator iter = iterC->begin(); iter != iterC->end(); iter++)
    	{
        	const int index = (barcode_index_p->find(iter->first))->second;
        	for(vector<int>::const_iterator iter2 = (bar_unikmer_p->begin()+index)->begin(); iter2 != (bar_unikmer_p->begin()+index)->end(); iter2++)
        	{
            	if(unikmer_freq.count(*iter2) > 0)
            	{
                	unikmer_freq.find(*iter2)->second += 1;
            	}else
            	{
                	unikmer_freq.insert(make_pair(*iter2,1));
            	}
        	}
    	}
		int minUnikmerFreq = cutoff2;
		double minRelation = cutoff1;
		for(unordered_map<int,int>::iterator iterF = unikmer_freq.begin(); iterF != unikmer_freq.end(); )
    	{
        	if(iterF->second <= minUnikmerFreq)
        	{
            	unikmer_freq.erase(iterF++);
        	}else
        	{
            	++iterF;
        	}
   	 	}
		//cerr << "unique kmer freq finish" << endl;

		vector<int> assistant_list;
		if(debug)
		{
			temOut << "k = 0 : " << endl;
			for(unordered_map<int, int>::iterator iter = Tbarcode_list.begin(); iter != Tbarcode_list.end(); iter++)
			{
				temOut << iter->first << "\t";
			}
			temOut << endl;
		}
		for(int kTime = 1; kTime != k; kTime++)
		{
			//3.1	get optional barcode
			subOpt_list.clear();
			for(unordered_map<int,int>::const_iterator iter1 = Tbarcode_list.begin(); iter1 != Tbarcode_list.end(); iter1++)
    		{
        		if((this->relate_hash_p)->count(iter1->first) > 0 )
        		{
            		unordered_map<int, vector<int> >::const_iterator iter2 = (this->relate_hash_p)->find(iter1->first);
            		for(vector<int>::const_iterator iter3 = iter2 -> second.begin(); iter3 != iter2 -> second.end(); iter3++)
           			 {
                		unordered_map<int,int>::const_iterator iter4 = Tbarcode_list.find(*iter3);
                		unordered_map<int,int>::const_iterator iter5 = subOpt_list.find(*iter3);
                		if(iter4 == Tbarcode_list.end() && iter5 == subOpt_list.end())
                		{
                   			subOpt_list.insert(std::make_pair(*iter3, 1));
                		}
            		}
        		}else
        		{
            		cerr << "In GetOptionalBarcode step: cannot find " << iter1->first << " in relate_hash" << endl;
					return false;
        		}
				//cerr << "get optional barcode finish" << endl;
			
			}
			//3.2	expand barcode
			assistant_list.clear();
		    //add the right barcode into the assistant_list
		    for(unordered_map<int,int>::iterator iter = subOpt_list.begin(); iter != subOpt_list.end(); iter++)
		    {
		        int co_unikmer = 0;
		        const int index = (barcode_index_p->find(iter->first)) ->second;
		        for(vector<int>::const_iterator iter2 = (bar_unikmer_p->begin()+index)->begin(); iter2 != (bar_unikmer_p->begin()+index)->end(); iter2++)
		        {
		        	//double minUnikmerNum = std::min(barcode_info_map.at(iter->first).unikmerNum, (double)unikmer_freq.size());
		            if(unikmer_freq.find(*iter2) != unikmer_freq.end())
		            {
		                co_unikmer ++;
		        	}
		             if(co_unikmer  >= (int)minRelation)
		            {
		                assistant_list.push_back(iter->first);
		                break;
		            }
		        }
		    }
		    for(vector<int>::iterator iter3 = assistant_list.begin(); iter3 != assistant_list.end(); iter3++)
		    {
		        Tbarcode_list.insert(make_pair(*iter3, 1));
		    }
		    //updata the unique kmer frequence
		    for(vector<int>::iterator iter = assistant_list.begin(); iter != assistant_list.end(); iter++)
		    {
		        const int index = (barcode_index_p->find(*iter))->second;
		        for(vector<int>::const_iterator iter2 = (bar_unikmer_p->begin()+index)->begin(); iter2 != (bar_unikmer_p->begin()+index)->end(); iter2++)
		        {
		            if(unikmer_freq.count(*iter2) > 0)
		            {
		                unikmer_freq.find(*iter2)->second += 1;
		            }else
		            {
		                unikmer_freq.insert(make_pair(*iter2,1));
		            }
		        }
		    }
			//cerr << "expand barcode finish" << endl;
			//3.3	remove bad unique kmer
			if(selectModel)
			{
				for(unordered_map<int,int>::iterator iter = unikmer_freq.begin(); iter != unikmer_freq.end(); )
			    {
			        if(iter->second <= minUnikmerFreq)
			        {
			            unikmer_freq.erase(iter++);
			        }else
			        {
			            ++iter;
			        }
			    }
			//cerr << "remove bad unique kmer finish" << endl;
			minUnikmerFreq += add;
			}
			if(debug)
			{
				temOut << "k = " << kTime << " : " << endl;
				for(unordered_map<int, int>::iterator iter = Tbarcode_list.begin(); iter != Tbarcode_list.end(); iter++)
				{
					temOut << iter->first << "\t";
				}
				temOut << endl;
			}
		}
		//add Tbarcode_list to subcluster
		for(unordered_map<int, int>::iterator iterT = Tbarcode_list.begin(); iterT != Tbarcode_list.end(); iterT++)
		{
			if(iterC->find(iterT->first) == iterC->end())
			{
				iterC->insert(make_pair(iterT->first, 1));
			}
		}
	}
	return true;
}


