#include "Barcode2Read.h"
#include "SeqOperator.h"
#include "Util.h"
#include <string>

#define MaxKmer 32
#define ReadLength 100


void Barcode2Read::add(SeqRecord& sr1, SeqRecord& sr2)
{
    PairInBinary pread;
    string s1_1 = sr1.seq.substr(0,MaxKmer);
    string s1_2 = sr1.seq.substr(MaxKmer,MaxKmer);
    string s1_3 = sr1.seq.substr(2*MaxKmer,MaxKmer);
    string s1_4 = sr1.seq.substr(3*MaxKmer,ReadLength-3*MaxKmer);
    string s2_1 = sr2.seq.substr(0,MaxKmer);
    string s2_2 = sr2.seq.substr(MaxKmer,MaxKmer);
    string s2_3 = sr2.seq.substr(2*MaxKmer,MaxKmer);
    string s2_4 = sr2.seq.substr(3*MaxKmer,ReadLength-3*MaxKmer);
    pread.read_P1.read_1 = Seq2Kmer(s1_1);
    pread.read_P1.read_2 = Seq2Kmer(s1_2);
    pread.read_P1.read_3 = Seq2Kmer(s1_3);
    pread.read_P1.read_4 = Seq2Kmer(s1_4);
    pread.read_P2.read_1 = Seq2Kmer(s2_1);
    pread.read_P2.read_2 = Seq2Kmer(s2_2);
    pread.read_P2.read_3 = Seq2Kmer(s2_3);
    pread.read_P2.read_4 = Seq2Kmer(s2_4);
    if(this->hash_index.find(sr1.barcode) != hash_index.end())
    {
        this->hash_index.at(sr1.barcode).push_back(pread);
    }else
    {
        vector<PairInBinary> vec;
        vec.push_back(pread);
        this->hash_index.insert(pair<int, vector<PairInBinary> >(sr1.barcode, vec));
    }
}

unordered_map<Barcode,vector<PairInBinary> >* Barcode2Read::get_hash()
{
    return &(this->hash_index);
}
