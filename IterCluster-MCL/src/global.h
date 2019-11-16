#include <unordered_map>
#include <string>
#include <vector>
#include <map>
#ifndef _GLOBAL_H_
#define _GLOBAL_H_
struct BarcodeInfo
{
	double unikmerNum;
	double kmerNum;
	double unikmerRate;
};

extern std::unordered_map<int,std::vector<int> > relate_seed_hash;
extern std::unordered_map<int,int> barcode_index;
extern std::unordered_map<int, BarcodeInfo > barcode_info_map;
extern std::vector<std::vector<int> > bar_unikmer_set;
extern std::vector<std::vector<int> >tar_barcode;
//declare parameter
extern std::string relation_matrix_path;
extern std::string barcode_info_path;
extern std::string barcode_to_kmer_path;
extern int matrix_cut;
extern int k;
extern int unique_num;
extern double unique_rate;
extern double cutoff1;
extern int cutoff2;
extern int thread_num;
extern int add;
extern int iterTime;
extern int selectModel;
extern int debug;


#endif

