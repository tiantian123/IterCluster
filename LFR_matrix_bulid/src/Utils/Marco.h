#ifndef __LFR_UTILS_MARCO_H__
#define __LFR_UTILS_MARCO_H__

#include "Define.h"
#include <stdint.h>
#include <string.h>
#include <map>
#include <iostream>

#if DEBUG

#endif

//////////////////////////////////////////////////////////////////////


#ifndef KVALUE 
#define KVALUE 17
#endif

#ifndef BUFFMAX
#define BUFFMAX 10000
#endif


/////////////////////////////////////////////////////////////////////

#define KBITS (KVALUE *2)
#define KMASK ((0x1UL <<( KBITS)) -1)


/////////////////////////////////////////////////////////////////////


inline int64_t KmerIndex(const char * start , size_t len, std::map < uint64_t, std::string >& subKmerSeq2ID)
{
#if DEBUG
    static int count = 0 ;
    if(start == nullptr)
    {
        std::cout<<"Total kmer calc  "<<count<<std::endl;
        return -1;
    }
    count ++ ;
#endif
    uint64_t l2r = 0 ;
    uint64_t r2l = 0 ;
    const char * end = start+len-1;
    for(size_t i = 0 ; i < len ; i++)
    {
        l2r = (l2r<<2) + (((*start) & 0x6)>>1);
        r2l = (r2l<<2) + ((((*end) ^ 0x4)>>1) & 0x3);
        end --;
        start++;
    }
	//wjc 2019-01-25
	
	uint64_t kmerID = l2r < r2l ? l2r : r2l;
	char KmerSeq[len + 1];
	KmerSeq[0] = '\0';
	start = start - len;
	//std::cerr << KmerSeq[0] << std::endl;
	strncpy(KmerSeq, start, len);
	//std::cerr << KmerSeq[0] << std::endl;
	std::string s1(KmerSeq, strlen(KmerSeq));
	//std::cerr << "start to print kmerID 2 KmerSeq" << std::endl;
	//std::cerr << kmerID << "\t" << s1 << std::endl;
	//std::cerr << s1 << std::endl;
	if(subKmerSeq2ID.find(kmerID) == subKmerSeq2ID.end())
	{
		subKmerSeq2ID.insert(std::pair<uint64_t, std::string>(kmerID, s1));
	}

    return l2r < r2l ? l2r : r2l ;
}



#endif //__LFR_UTILS_MARCO_H__
