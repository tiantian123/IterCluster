#include "SeqOperator.h"

using namespace std;

const char alphabet[256] =
{
 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 4, 0, 4, 1, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4,
 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 4, 0, 4, 1, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4,
 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
};


const char sbin[4] ={
	'A', 'C', 'G', 'T'
};

//change string type seq to binary uint64_t
uint64_t Seq2Kmer(string &seq)
{
    uint64_t mer=0;
	for(int i=0; i<seq.size(); i++) {
		mer=(mer<<2)|alphabet[seq[i]];
	}
	return mer;
}

//change binary uint64_t to string type seq
string Kmer2Seq(uint64_t mer, int kmer)
{
    	string seq;
	for(int i=0; i<kmer; i++) {
		seq.push_back(sbin[(mer>>(kmer-1-i)*2)&0x3]);
	}
	return seq;
}

//change all binary reads to 100 string
string ReadsInBinary2str(ReadsInBinary binary_read)
{
    string str;
    str += Kmer2Seq(binary_read.read_1, 32);
    str += Kmer2Seq(binary_read.read_2, 32);
    str += Kmer2Seq(binary_read.read_3, 32);
    str += Kmer2Seq(binary_read.read_4, 4);
    return str;
}



