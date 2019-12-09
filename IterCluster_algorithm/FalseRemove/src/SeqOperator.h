#ifndef SEQOPERATOR_H_INCLUDED
#define SEQOPERATOR_H_INCLUDED
#include <stdint.h>
#include <string>
#include "ReadsInBinary.h"

using namespace std;
uint64_t Seq2Kmer(string &seq);
string Kmer2Seq(uint64_t mer, int kmer);
string ReadsInBinary2str(ReadsInBinary binary_read);

#endif // SEQOPERATOR_H_INCLUDED
