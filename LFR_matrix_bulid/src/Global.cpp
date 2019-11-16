#include "Global.h"
//
// All default value here.
//

int KValue = 17;
int ThreadNum = 8;

bool UseFullKmerArray = false;  // Use FullKmerArray or KmerHash
bool BinaryOutput = false;  // Use BinaryOutput

FullKmerHash* The_FullKmerHash = nullptr;
KmerHash  * The_KmerHash = nullptr;
UniqueKmerArray * The_UniqueKmerArray = nullptr;
std::vector<std::map<uint64_t, std::string> > kmerSeq2ID;


std::string file1;
std::string file2;
std::string file_kmer_count; //
std::string file_kmer_anlysis; //
std::string file_uniquekmer_index; // 
std::string file_barcode2kmer; // 
std::string file_barcode_info; // 
std::string file_barcode_matrix; //

int KCountMin = -1; //
int KCountMax = -1; //
long KIndexMax = -1; //

