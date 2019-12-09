#include <unordered_map>
#include <string>
#include <vector>
#include <map>
#include "global.h"

using namespace std;
unordered_map<int,vector<int> > relate_seed_hash;
unordered_map<int,int> barcode_index;
unordered_map<int, BarcodeInfo > barcode_info_map;
vector<vector<int> > bar_unikmer_set;
vector<vector<int> >tar_barcode;
//declare parameter
string relation_matrix_path;
string barcode_info_path;
string barcode_to_kmer_path;
int matrix_cut = 30;
int k = 4;
int unique_num = 1500;
double unique_rate = 0.3;
double cutoff1 = 200;
int cutoff2 = 3;
int thread_num = 20;
int add = 2;
int iterTime = 12;
int selectModel = 1;
int debug = 0;


