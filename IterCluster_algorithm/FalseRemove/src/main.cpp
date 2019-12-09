#include <iostream>
#include <fstream>
#include <string>
#include "LoadRead.h"
#include "Barcode2Read.h"
#include "ReadsInBinary.h"
#include "ClusterControl.h"
#include "Cluster.h"
#include <assert.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <mutex>

using namespace std;

void Usage()
{
    cout << "Usage: " << endl;
    cout << "-f" << "\t" << "input of read1" << endl;
    cout << "-q" << "\t" << "input of read2" << endl;
    cout << "-s" << "\t" << "input of cluster set" << endl;
    cout << "-k" << "\t" << "kmer length" << endl;
    cout << "-c" << "\t" << "cutoff" << endl;
    cout << "-d" << "\t" << "average sequence depth" << endl;
    cout << "-t" << "\t" << "thread number" << endl;
}

//global variable
string file1;
string file2;
string cluster_f;
int k;
int cutoff;
int ave_depth;
int thread_num;
pthread_key_t key;
std::mutex mtx;
//Multiple thread
struct param
{
    int k;
    int cutoff;
    Barcode2Read* Barcode2Read_p;
    unordered_map<Seed, vector<Barcode>>* ClusterSet_p;
    int ave_depth;
    ClusterControl* cluster_control_p;
    ofstream* outfq1_p;
    ofstream* outfq2_p;
};



void* My_thread(void* arg)
{
    //pthread_rwlock_rdlock(&rwlock);
    pthread_setspecific(key, arg);
    param* param_p = (param*)pthread_getspecific(key);
    while(!(param_p->cluster_control_p)->get_Empty())
    {
        mtx.lock();
        Seed seed = (param_p->cluster_control_p)->get_cluster();

        (param_p->cluster_control_p)->maskCluster(seed);

        mtx.unlock();
        //begin to FalseRemove
        Cluster cluster(seed, param_p->k, param_p->cutoff, *((param_p->Barcode2Read_p)->get_hash()), *(param_p->ClusterSet_p));
        cluster.Init();
        cluster.checkKmerreq(param_p->ave_depth);
        cluster.CountAllReadFreq();
        cluster.RemoveRead();
        mtx.lock();
        cerr << "Seed: " << seed << " finish!" << endl;
        cluster.PrintRead(*(param_p->outfq1_p), *(param_p->outfq2_p));
        //判断seed是否选择完，修改cluster_control
        (param_p->cluster_control_p)->set_Empty();
        mtx.unlock();
    }
    cout << "sub_pthread end" << endl;
    pthread_exit(NULL);


}
int main(int argc, char* const argv[])
{
    //ÅäÖÃÃüÁîÐÐ²ÎÊý
    int opt;
    int count_arg = 0;
    while((opt=getopt(argc, argv, "f:q:s:k:c:d:t:")) != -1)
    {
        switch(opt)
        {
        case 'f':
            file1 = optarg;
            break;
        case 'q':
            ++count_arg;
            file2 = optarg;
            break;
         case 's':
            ++count_arg;
            cluster_f = optarg;
            break;
        case 'k':
            ++count_arg;
            k = atoi(optarg);
            break;
        case 'c':
            ++count_arg;
            cutoff = atoi(optarg);
            break;
        case 'd':
            ++count_arg;
            ave_depth = atoi(optarg);
        case 't':
            ++count_arg;
            thread_num = atoi(optarg);
        default: /* '?' */
            Usage();
            exit(-1);
            break;
        }
    }
    if(count_arg < 6)
    {
        Usage();
        exit(-1);
    }
    ofstream outfq1("output.read1");
    ofstream outfq2("output.read2");
    assert(outfq1.is_open());
    assert(outfq2.is_open());
    Barcode2Read bar2read;
    LoadRead2Binary(file1,file2,bar2read);
    cerr << "LoadRead2Binary success" << endl;
    unordered_map<Seed,vector<Barcode> > ClusterSet = LoadClusterSet(cluster_f);
    cerr << "LoadClusterSet success" << endl;
    ClusterControl cluster_control(ClusterSet);
    pthread_key_create(&key, NULL);
    pthread_t tids[thread_num];
    for(int i = 0; i != thread_num; i++)
    {
        //set the threading parameter
        param *param_p = new param;
        param_p->ave_depth = ave_depth;
        param_p->Barcode2Read_p = &bar2read;
        param_p->ClusterSet_p = &ClusterSet;
        param_p->cluster_control_p = &cluster_control;
        param_p->cutoff = cutoff;
        param_p->k = k;
        param_p->outfq1_p = &outfq1;
        param_p->outfq2_p = &outfq2;
        int ret = pthread_create(&tids[i], NULL, My_thread, param_p);
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
    return 0;
}
