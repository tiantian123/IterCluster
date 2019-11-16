#include "ClusterOperator.h"
#include <unordered_map>
#include <fstream>
#include "Timer.h"
#include <stdint.h>
#include "ReadsInBinary.h"
#include "SeqOperator.h"
#include <fstream>

#define ReadLength 100
//#define DEBUG
using namespace std;

void AddRead2freq(unordered_map<uint64_t, int> kmerfreq, string read, int k)
{
    for(int i = 0; i != ReadLength-k+1; i++)
    {
        string seq = read.substr(i,k);
        uint64_t kmer = Seq2Kmer(seq);
        if(kmerfreq.find(kmer) == kmerfreq.end())
        {
            kmerfreq.insert(pair<uint64_t,int>(kmer,1));
        }else
        {
            kmerfreq.at(kmer) ++;
        }
    }
}

void InitKmerfreq(Seed seed, unordered_map<Barcode,vector<PairInBinary> >& hash_index, unordered_map<Seed,vector<Barcode> >& seed2barcode, int k)
{
     unordered_map<uint64_t, int> kmerfreq;
     for(vector<Barcode>::iterator iter = seed2barcode.at(seed).begin(); iter != seed2barcode.at(seed).end(); iter++)
     {
         for(vector<PairInBinary>::iterator iter2 = hash_index.at(*iter).begin(); iter2 != hash_index.at(*iter).end(); iter2++)
         {
             string read1 = ReadsInBinary2str(iter2->read_P1);
             string read2 = ReadsInBinary2str(iter2->read_P2);
             AddRead2freq(kmerfreq, read1, k);
             AddRead2freq(kmerfreq, read2, k);
         }
     }
     #ifdef DEBUG
     ofstream debug("debug_kmerfreq");
     assert(debug.is_open());
     for(unordered_map<uint64_t, int>::iterator iter = kmerfreq.begin(); iter != kmerfreq.end(); iter++)
     {
         debug << iter->first << "\t" << iter->second << endl;
     }
     debug.close();
     #endif // DEBUG
}






