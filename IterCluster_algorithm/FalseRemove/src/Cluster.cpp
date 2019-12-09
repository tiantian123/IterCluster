#include "Cluster.h"
#include <unordered_map>
#include <fstream>
#include "Timer.h"
#include <stdint.h>
#include "ReadsInBinary.h"
#include "SeqOperator.h"
#include <fstream>
#include <algorithm>
#include <string>

#define ReadLength 100
//#define DEBUG
using namespace std;

void Cluster::AddRead2freq(string read)
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

void Cluster::checkKmerreq(int ave_depth)
{
    for(unordered_map<uint64_t, int>::iterator iter = this->kmerfreq.begin(); iter != this->kmerfreq.end(); iter++)
    {
        if(iter->second >= ave_depth * 2)
        {
            iter->second = ave_depth * 2;
        }
    }
}

void Cluster::Init()
{
	Timer t("Init");
     for(vector<Barcode>::iterator iter = seed2barcode_p->at(seed).begin(); iter != seed2barcode_p->at(seed).end(); iter++)
     {
         for(vector<PairInBinary>::iterator iter2 = hash_index_p->at(*iter).begin(); iter2 != hash_index_p->at(*iter).end(); iter2++)
         {
             string read1 = ReadsInBinary2str(iter2->read_P1);
             string read2 = ReadsInBinary2str(iter2->read_P2);
             AddRead2freq(read1);
             AddRead2freq(read2);
             PairInBinary *p =nullptr;
             p = &(*iter2);
             this->read2count.insert(pair<PairInBinary*,int>(p,0));
         }
     }
     #ifdef DEBUG
     string outfile = "debug_kmerfreq_" + to_string(seed);
     ofstream debug(outfile);
     assert(debug.is_open());
     for(unordered_map<uint64_t, int>::iterator iter = kmerfreq.begin(); iter != kmerfreq.end(); iter++)
     {
         debug << iter->first << "\t" << iter->second << endl;
     }
     debug.close();
     #endif // DEBUG
}

void Cluster::CountAllReadFreq()
{
	Timer t("CountAllReadFreq");
    #ifdef DEBUG
    string outfile = "debug_" + to_string(seed);
    ofstream debug(outfile);
    int Count = 0;
    #endif // DEBUG
    for(unordered_map<PairInBinary*,int>::iterator iter = read2count.begin(); iter != read2count.end(); iter++)
    {
        string read1 = ReadsInBinary2str((iter->first)->read_P1);
        string read2 = ReadsInBinary2str((iter->first)->read_P2);
        int p1count = CountReadFreq(read1);
        int p2count = CountReadFreq(read2);
        iter->second = max(p1count,p2count);
        #ifdef DEBUG
        debug << Count << "\t" << iter->second << read1 << "\t" << p1count << "\t" << read2 << "\t" << p2count << "\t" << seed << endl;
        Count ++;
        #endif // DEBUG
    }
    #ifdef DEBUG
    debug.close();
    #endif // DEBUG
}

int Cluster::CountReadFreq(string read)
{
    int num = 0;
    //cerr << "A new read :" << endl;
    for(int i = 0; i != ReadLength - k + 1; i++)
    {
        string seq = read.substr(i, k);
        uint64_t kmerBinary = Seq2Kmer(seq);
        num += kmerfreq.at(kmerBinary);
        //cerr << seq << ": " << kmerfreq.at(kmerBinary) << endl;
    }
    //cerr << "read tital count: " << num << endl;
    return num;
}

void Cluster::RemoveRead()
{
	Timer("RemoveRead");
    for(unordered_map<PairInBinary*,int>::iterator iter = read2count.begin(); iter != read2count.end();)
    {
        int aveCount= (int)(iter->second / (ReadLength - k + 1));
        if(aveCount < cutoff)
        {
            read2count.erase(iter++);
        }else{
            ++iter;
        }
    }
}

void Cluster::PrintRead(ofstream& fq1, ofstream& fq2)
{
    for(unordered_map<PairInBinary*,int>::iterator iter = read2count.begin(); iter != read2count.end(); iter++)
    {
        string read1 = ReadsInBinary2str((iter->first)->read_P1);
        string read2 = ReadsInBinary2str((iter->first)->read_P2);
        fq1 << "@" << seed << "\n";
        fq1 << read1 << "\n";
        fq2 << "@" << seed << "\n";
        fq2 << read2 << "\n";
    }
}

