#ifndef CLUSTER_H
#define CLUSTER_H
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include "ReadsInBinary.h"

using namespace std;

typedef int Seed;
typedef int Barcode;
class Cluster
{
    public:
        Cluster(Seed s, int kmer, int c, unordered_map<Seed,vector<PairInBinary> >& hash_index, unordered_map<Seed,vector<Barcode> >& seed2barcode):seed(s),k(kmer),cutoff(c)
        {
            hash_index_p = &hash_index;
            seed2barcode_p = &seed2barcode;
        };
        void Init();
        void AddRead2freq(string read);
        void checkKmerreq(int ave_depth);
        int CountReadFreq(string read);
        void CountAllReadFreq();
        void RemoveRead();
        void PrintRead(ofstream& fq1, ofstream& fq2);
    private:
        unordered_map<uint64_t, int> kmerfreq;
        unordered_map<PairInBinary*,int> read2count;
        unordered_map<Seed,vector<PairInBinary> >* hash_index_p;
        unordered_map<Seed,vector<Barcode> >* seed2barcode_p;
        Seed seed;
        int k;
        int cutoff;
};

#endif // CLUSTER_H
