#ifndef __LFR_LOGIC_H__
#define __LFR_LOGIC_H__
#include <string>
#include "Define.h"

void printHash(const std::string & name);
void loadHash(const std::string & name);

// 
// load reads from file,
// construct kmer count hash table.
//
void Read2KmerCount(const std::string & filename,
                    int thread_num);

void PrintKmerCount();
void LoadKmerCount();
//
// exclude kmers that not in [min,max]
// index left from 1 - n .
//
uint64_t IndexKmers(int min, int max);
void PrintUniqueKmer();
void LoadUniqueKmer();

//
// load reads from file ,
// construct Kmer to Barcode table,
// write kmers for each Barcode into outf.
//
void Kmer2BarcodeHash();
    /*const std::string &f1, 
                      const std::string &f2,
                      int thread_num,
                      const std::string &outf);*/

void LoadKmer2BarcodeHash();
//
// load kmers for each barcode
// calculate barcode mutrix.
//
void BarcodeMutrix();/*const std::string &inf,
                   const std::string &outf,
                   int thread_num);*/




#endif //__LFR_LOGIC_H__
