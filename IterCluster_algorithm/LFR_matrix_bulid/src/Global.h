#ifndef __LFR_GLOBAL_H__
#define __LFR_GLOBAL_H__
#include <stdint.h>
#include "KmerHash.h"
#include "UniqueKmerArray.h"
#include <map>
#include <string>
#include <vector>
///
// Declare all global variable here.
//
// All defined in Global.cpp
///

extern int KValue;  // K 
extern int ThreadNum;   // Job thread num
extern bool UseFullKmerArray;  // Use FullKmerArray or KmerHash
extern bool BinaryOutput;  // Use BinaryOutput

extern std::string file1; // read P
extern std::string file2; // read E

extern std::string file_kmer_count; //
extern std::string file_kmer_anlysis; //
extern std::string file_uniquekmer_index; // 
extern std::string file_barcode2kmer; // 
extern std::string file_barcode_info; // 
extern std::string file_barcode_matrix; //

extern int KCountMin; //
extern int KCountMax; //
extern long KIndexMax; //

//////////////////////////////////////////////////////

extern FullKmerHash* The_FullKmerHash;
extern KmerHash * The_KmerHash;
extern UniqueKmerArray * The_UniqueKmerArray;

//wjc bulid kmerSeq -> kmerID hash;
extern std::vector<std::map<uint64_t, std::string> > kmerSeq2ID;

#endif
