#ifndef SEED_H
#define SEED_H
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
#include "global.h"


using namespace std;
////////////////////////////////
////////////////////////////////
////Seed is the cluster unit////
////////////////////////////////
////////////////////////////////

class Seed
{
public:
    Seed(const unordered_map<int, vector<int > >&, const vector<vector<int> >&, const unordered_map<int, int>&);
    void set_seed(int barcode);
    vector<unordered_map<int, int> >* GetSubClusterList();
    void PrintSeedList(ofstream& output);
    int GetSeedBarcode();
	vector<unordered_map<int, int> >* GetSubCluster;
	unordered_map<int, int>* GetSeedList();
	void clear();
	void addToSeedList(int);
	bool clustering(ofstream&);
private:
    int seed_barcode;
	unordered_map<int, int> seed_list;
    vector<unordered_map<int, int> > subCluster;//barcode����ID
    const unordered_map<int, vector<int > > *relate_hash_p;//��ϵ������Ϣ�� key��barcode����ID�� value����key��ص�barcode����ID��
    const vector<vector<int> > *bar_unikmer_p;//barcode��uniquekmer������Ϣ�� index1:barcode����ID; value:uniquekmer����ID��
    const unordered_map<int, int> *barcode_index_p;//barcode������index��Ϣ�� key:barcode����ID�� value��barcode����ID
};

#endif // SEED_H

