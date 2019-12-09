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
#include <Seed.h>
#include <SeedControl.h>
#include <Timer.h>
#include <assert.h>
#include "global.h"
using namespace std;

//��ȡ��ϵ����
void GetMatrix(unordered_map<int, vector<int > >& relate_seed_hash, int cutoff1, ifstream& in_Matrix)
{
	cerr << "Start to get matrix" << endl;
    Timer t("GetRelativeMatrix");
    //ofstream out("out");
    //develop a hash to save relation
    string s;
    while(getline(in_Matrix,s)){
        vector<int> tar_barcode_array;
        istringstream strm(s);
        string seed;
        getline(strm,seed,':');
		//cerr << seed << ":";
        int Seed = atof(seed.c_str());
        string unikmer_num;
        getline(strm,unikmer_num,' ');
        int Unikmer_num = atof(unikmer_num.c_str());
        string tar_barcode;
        while(getline(strm,tar_barcode,':'))
        {
            int Tar_barcode = atof(tar_barcode.c_str());
            string co_rate;
            getline(strm,co_rate,' ');
            double Co_rate = atof(co_rate.c_str());
            int Co_num = (int)(Co_rate * Unikmer_num);
			//change co_rate
			//Co_rate = Co_num / std::min(barcode_info_map.at(Seed).unikmerNum, barcode_info_map.at(Tar_barcode).unikmerNum);
            if(Co_num >= cutoff1)
            {
            	//cerr << "\t" << Co_rate;
                tar_barcode_array.push_back(Tar_barcode);
            }
        }
		//cerr << endl;

            relate_seed_hash.insert(make_pair(Seed, tar_barcode_array));
    }
    in_Matrix.close();

        /*//output the hash
        for(map<int, vector<int> >::iterator iter = relate_seed_hash.begin(); iter != relate_seed_hash.end(); iter ++){
                out << iter -> first << "\t";
                vector<int>::iterator itervec = iter -> second.begin();
                for(; itervec != iter -> second.end(); itervec++){
                        out << (*itervec) << "\t";
                }
                out << "\n";
        }*/

}

void SplitString(const string& s, vector<double>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back((double)atol(s.substr(pos1, pos2 - pos1).c_str()));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back((double)atol(s.substr(pos1).c_str()));
}



//get the bar_ID index table and bar_unikmerID table.
void LoadIndex(unordered_map<int, int>& barcode_index, vector<vector<int> >& bar_unikmer_set, ifstream& in_BarToKmer)
{
    Timer t("LoadBarcodeToUniquekmer");
	long  index = 0;
	string s;
	bar_unikmer_set.clear();
	barcode_index.clear();
	vector<int> unikmer_array;
	while (getline(in_BarToKmer, s))
	{
		istringstream strm(s);
		string barcode;
		string unikmer;
		string unikmer_num;
		unikmer_array.clear();
		strm >> barcode;
		barcode_index[atol(barcode.c_str())] = index;
		strm >> unikmer_num;//��1ng����ʱ���ע��
		//strm >> unikmer_string;
		//cout << unikmer_string << endl;
		//if (unikmer_string.c_str())
		while(getline(strm, unikmer, ' '))
        {
            unikmer_array.push_back(atol(unikmer.c_str()));

		}
		bar_unikmer_set.push_back(unikmer_array);
		++index;
	}
	in_BarToKmer.close();
}


/////////////////////////////////////
/////////////////////////////////////
/////////////global////////////////
/////////////////////////////////////
/////////////////////////////////////
SeedControl seed_control;
//�߳�ȫ�ֱ���
pthread_key_t key;
std::mutex mtx;


struct para
{
	int outputID;
};
///////////////////////////////////////
///////////////////////////////////////
///////////////////////////////////////
//////////Multithreading///////////////
///////////////////////////////////////
///////////////////////////////////////
///////////////////////////////////////
/*
void* My_thread(void* arg)
{
    //pthread_rwlock_rdlock(&rwlock);
    pthread_setspecific(key, arg);
    param* param_p = (param*)pthread_getspecific(key);
    while(!(param_p->seed_control_p)->get_Empty())
    {
        //pthread_rwlock_unlock(&rwlock);
        //pthread_rwlock_wrlock(&rwlock);
        mtx.lock();
        //cerr << "Thread up seed:" << param_p->barcode << endl;
        param_p->barcode = (param_p->seed_control_p)->get_seed();
        //debug
        //cerr << "Thread next seed:" << param_p->barcode << endl;
        (param_p->seed_control_p)->mask_seed(param_p->barcode);
        //cerr << "mask_seed " << param_p->barcode << " success" << endl;
        param_p->seed.set_seed(param_p->barcode);
        //cerr << "set_seed " << param_p->barcode << " success" << endl;
        //debug
        //for(unordered_map<int,int>::iterator iter = param_p->seed.GetUnikmer_freq()->begin(); iter != param_p->seed.GetUnikmer_freq()->end(); iter++)
        //{
        //    cerr << iter->first << "\t" << iter->second << endl;
        //}
        mtx.unlock();
        //pthread_rwlock_rdlock(&rwlock);
        for(int i = 0; i!=param_p->k; i++)
        {
            (param_p->seed).GetOptBar();
            (param_p->seed).ExpandSeed(param_p->cutoff1);
            (param_p->seed).RemoveBadUnikmer(param_p->cutoff2);
            param_p->cutoff2 += 2;
        }
	param_p->cutoff2 = cutoff2;
        //pthread_rwlock_unlock(&rwlock);
        //pthread_rwlock_wrlock(&rwlock);
        mtx.lock();
        cerr << "Seed: " << param_p->barcode << "\t" << endl;
        for(unordered_map<int,int>::iterator iter = ((param_p->seed).GetSeedList())->begin(); iter != ((param_p->seed).GetSeedList())->end(); iter++)
        {
            (param_p->seed_control_p)->mask_seed(iter->first);
            (param_p->tar_vec).push_back(iter->first);
            cerr << iter->first << "\t";
        }
        cerr << "\n";
        map<int, vector<int> >::iterator iter = (param_p->tar_barcode_p)->find(param_p->barcode);
        (iter->second).assign((param_p->tar_vec).begin(), (param_p->tar_vec).end());
        param_p->tar_vec.clear();
        (param_p->seed_control_p)->set_Empty();
        //pthread_rwlock_unlock(&rwlock);
        //pthread_rwlock_rdlock(&rwlock);
        mtx.unlock();
    }
    cout << "sub_pthread end" << endl;
    pthread_exit(NULL);


}
*/

void* My_thread2(void* arg)
{
	para *pa_p = (para *)arg;
	string str1 = "optput";
    string outputID = to_string(pa_p->outputID);
	string fq = str1 + outputID;
	ofstream temOut(fq);
    assert(temOut.is_open());
	while(!seed_control.get_Empty())
    {
        mtx.lock();
        int barcode1 = seed_control.get_seed();
		seed_control.mask_seed(barcode1);
		int barcode2 = seed_control.get_seed();
		seed_control.mask_seed(barcode2);
		if(barcode1 != barcode2 && barcode1 != 0 && barcode2 != 0)
		{
			Seed seed = Seed(relate_seed_hash, bar_unikmer_set, barcode_index);
	        seed.set_seed(barcode1);
			seed.addToSeedList(barcode2);
	        mtx.unlock();
			if(!seed.clustering(temOut))
			{
				cerr << "seed: " << barcode1 << "\t" << barcode2 << "clustering fail" << endl;
			}
	        mtx.lock();
			temOut << barcode1 << "\t" << barcode2 << "\t";
	        for(vector<unordered_map<int, int> >::iterator iter = seed.GetSubClusterList()->begin(); iter != seed.GetSubClusterList()->end(); iter++)
	        {
	        	vector<int> tar_vec;
				tar_vec.push_back(barcode1);
				tar_vec.push_back(barcode2);
	        	for(unordered_map<int, int>::iterator iter2 = iter->begin(); iter2 != iter->end(); iter2++)
	        	{
	            	seed_control.mask_seed(iter2->first);
	            	tar_vec.push_back(iter2->first);
	            	//temOut << iter2->first << "\t";
	        	}
				//temOut << "\n";
				//2018-06-09 just for a bug I can't stop
				seed_control.set_Empty();
				if(!seed_control.get_Empty())
				{
					tar_barcode.push_back(tar_vec);
				}
	        }
			seed.clear();
	        seed_control.set_Empty();
			mtx.unlock();
		}
		else
		{
			mtx.unlock();
			break;
		}
    }
    cout << "sub_pthread end" << endl;
    pthread_exit(NULL);
}



void Usage()
{
    cout << "Usage: " << endl;
    cout << "-m" << "\t" << "the input of relation matrix[path]" << endl;
    cout << "-i" << "\t" << "the input of barcode information index[path]" << endl;
    cout << "-b" << "\t" << "the input of barcode to kmer index[path]" << endl;
    cout << "-c" << "\t" << "the minimum co-unique kmer in the relation matrix[int]" << endl;
    cout << "-k" << "\t" << "the iteration time for each seed when cluster[int]" << endl;
    cout << "-n" << "\t" << "the minimum unique kmer number of the seed[int]" << endl;
    cout << "-r" << "\t" << "the minimum unique kmer rate of the seed[double]" << endl;
    cout << "-g" << "\t" << "the minimum co-unique kmer between seed and target barcode[int]" << endl;
    cout << "-f" << "\t" << "the minimum frequency of target unique kmer[int]" << endl;
    cout << "-t" << "\t" << "the thread number[int]" << endl;
	cout << "-a" << "\t" << "the unique kmer frequency increment for each[int] " << endl;
	cout << "-h" << "\t" << "the iteration time of MCL[int]" << endl;
	cout << "-s" << "\t" << "whether to use frequency select model" << endl;
	cout << "-d" << "\t" << "whether to debug" << endl;
}



int main(int argc, char* const argv[])
{
    //���������в���
    int opt;
    int count_arg = 0;
    while((opt=getopt(argc, argv, "m:i:b:c:k:n:r:g:f:t:a:h:s:d:")) != -1)
    {
        switch(opt)
        {
        case 'm':
            relation_matrix_path = optarg;
            break;
        case 'i':
            ++count_arg;
            barcode_info_path = optarg;
            break;
        case 'b':
            ++count_arg;
            barcode_to_kmer_path = optarg;
            break;
        case 'c':
            ++count_arg;
            matrix_cut = atoi(optarg);
            break;
        case 'k':
            ++count_arg;
            k = atoi(optarg);
            break;
        case 'n':
            ++count_arg;
            unique_num = atoi(optarg);
            break;
        case 'r':
            ++count_arg;
            unique_rate = atof(optarg);
            break;
        case 'g':
            ++count_arg;
            cutoff1 = atof(optarg);
            break;
        case 'f':
            ++count_arg;
            cutoff2 = atoi(optarg);
            break;
        case 't':
            ++count_arg;
            thread_num = atoi(optarg);
            break;
		case 'a':
			++count_arg;
			add = atoi(optarg);
			break;
		case 'h':
			++count_arg;
			iterTime = atoi(optarg);
			break;
		case 's':
			++count_arg;
			selectModel = atoi(optarg);
			break;
		case 'd':
			++count_arg;
			debug = atoi(optarg);
			break;
        default: /* '?' */
            Usage();
            exit(-1);
            break;
        }
    }
    if(count_arg < 13)
    {
        Usage();
        exit(-1);
    }
	
	ifstream in_BarInfo(barcode_info_path);
    assert(in_BarInfo.is_open());
    ifstream in_Matrix(relation_matrix_path);
    assert(in_Matrix.is_open());
    ifstream in_BarToKmer(barcode_to_kmer_path);
    assert(in_BarToKmer.is_open());
	LoadIndex(barcode_index, bar_unikmer_set, in_BarToKmer);
	//bulid seed_control
	seed_control.init(unique_rate, unique_num);
	seed_control.LoadBarcodeInfo(in_BarInfo);
	GetMatrix(relate_seed_hash, matrix_cut, in_Matrix);
	//seed_control.LoadSeedList();
	//Multithreading
	pthread_key_create(&key, NULL);

    //debug
    //ofstream outfile("seed_control_map.debug");
    //for(unordered_map<int, int>::iterator iter = (param_t.seed_control_p->get_seed_control_map_p())->begin(); iter != (param_t.seed_control_p->get_seed_control_map_p())->end(); iter++)
    //{
    //    outfile << iter->first << "\t" << iter->second << endl;
    //}
    //outfile.close();
	//multithreading
	pthread_t tids[thread_num];
	for(int i = 0; i != thread_num; i++)
    {
    	para pa ;
		pa.outputID = i;
        int ret = pthread_create(&tids[i], NULL, My_thread2, (void*)&pa);
        if(ret != 0)
        {
            cerr << "pthread_create error: error_code=" << ret << endl;
        }
    }
    //wait for thread end
    for(int i = 0; i != thread_num; i++)
    {
        pthread_join(tids[i],NULL);
    }
    //output tar_barcode
    ofstream output("seed_tarbarcode.txt");
    assert(output.is_open());
    for(vector<vector<int> >::iterator iter = tar_barcode.begin(); iter != tar_barcode.end(); iter++)
    {
        if(!iter->empty())
        {
            for(vector<int>::iterator iter2 = iter->begin(); iter2 != iter->end(); iter2++)
            {
                output << *iter2 << "\t";
            }
            output << endl;
        }
    }
    output.close();
    pthread_exit(NULL);
	return 0;

}

