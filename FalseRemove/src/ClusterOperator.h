#ifndef CLUSTEROPERATOR_H_INCLUDED
#define CLUSTEROPERATOR_H_INCLUDED
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

typedef int Seed;
typedef int Barcode;
void InitKmerfreq(Seed seed, unordered_map<Seed,vector<Barcode> >& hash_index, unordered_map<Seed,Barcode>& seed2barcode, int kmer);
void AddRead2freq(unordered_map<uint64_t, int> kmerfreq, string read, int k);

#endif // CLUSTEROPERATOR_H_INCLUDED
