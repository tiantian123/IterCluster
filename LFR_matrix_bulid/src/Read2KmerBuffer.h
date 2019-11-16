#ifndef __LFR_READ2KMERBUFFER_H__
#define __LFR_READ2KMERBUFFER_H__

#include "Util.h"
#include "Marco.h"
#include "Global.h"

struct Read2KmerBuffer
{
    static const int BarcodeFlag01 = -2;
    static const int BarcodeFlag02 = -3;
    static const int BarcodeFlag03 = -4;

    SeqRecord *seqs ;//[BUFFMAX];
    uint64_t **kmers;//[BUFFMAX];
    int kmerlen ;

    Read2KmerBuffer():seqs(nullptr) , kmers(nullptr) ,kmerlen(-1)
    {
        seqs = new SeqRecord[BUFFMAX];
        kmers = new uint64_t*[BUFFMAX];
        for( int i = 0 ; i < BUFFMAX ; i++)
            kmers[i] = nullptr;
    }


    void Init(int klen)
    {
        if(kmerlen != -1)
            return ;
        kmerlen = klen;
        for( int i = 0 ; i < BUFFMAX ; i++)
        {
            kmers[i] = new uint64_t[kmerlen];
            memset(kmers[i] , 0 , sizeof(uint64_t) * kmerlen);
        }
    }

    ~Read2KmerBuffer()
    {
        delete [] seqs;
        for( int i = 0 ; i < BUFFMAX ; i++)
        { 
            if(kmers[i] != nullptr)
            { 
                delete [] kmers[i];
                kmers[i] = nullptr;
            }
        }
        delete [] kmers;
    }

    inline void chopRead2Kmer(int index)
    {
        uint64_t * kp = kmers[index];
        const char * start = seqs[index].seq.c_str();
        for( int i = 0 ; i < kmerlen; i++ )
        {
            kp[i] = KmerIndex(start+i,KValue,kmerSeq2ID[index]);
        }
    }
};

#endif
