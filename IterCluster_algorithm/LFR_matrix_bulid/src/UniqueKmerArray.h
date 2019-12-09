#ifndef  __LFR_UNIQUEKMERARRAY_H__
#define __LFR_UNIQUEKMERARRAY_H__

#include <string.h>
#include <cassert>
#include <iostream>
#include <stdint.h>
#include "Define.h"
/**********************************************************
 *
 * unique kmer id [0 len].
 * BarcodeCovMax = n 
 *
 *    +----------+----------------+- ****----+------------------+
 *    | count(0) | cov barcodeid 0| ........ |cov barcode id n  |
 *    +----------+----------------+- ****----+------------------+
 *       *
 *       *
 *       *
 *    +-----------+----------------+- ****----+-----------------+
 *    | count(len)| cov barcodeid 0| ........ |cov barcode id n |
 *    +-----------+----------------+- ****----+-----------------+
 *
 ***********************************************************/

struct UniqueKmerArray
{
    UniqueKmerArray(size_t knum , int maxk)
        :bytes(nullptr)
        ,num_kmer(knum)
        ,max_kmer_count(maxk)
    {
        std::cout<<"UniqueKmerArray Alloc size "<<num_kmer*(max_kmer_count +1) * sizeof(unsigned int)<<" byte"<<std::endl;
        bytes = new uint32_t[num_kmer*(max_kmer_count +1)];
        memset(bytes,0,num_kmer*(max_kmer_count +1) * sizeof(unsigned int));
    }

    ~UniqueKmerArray()
    {
        if(bytes)
            delete []bytes;
        bytes = nullptr;
    }

    void Append(size_t index , int id)
    {
        assert(index < num_kmer &&  *(bytes + index * (max_kmer_count +1))< max_kmer_count );
        uint32_t * pointer = bytes + index *(max_kmer_count +1 );
        uint32_t curr_size = *pointer;
        *(pointer+curr_size+1) = id;
        (*(pointer))+=1;
        /*for( uint32_t i = 0 ; i < curr_size ; i++)
        {
            pointer ++ ;
            if( *pointer == (uint32_t)id ) // Store 1 barcode only one time .
                return ;
        }
        *(++pointer) = id ;
        (*(bytes + index * (max_kmer_count +1)))++;*/
    }

    uint32_t GetBarcodeId(size_t index , int count)
    {
        return *(bytes + index * (max_kmer_count +1) + count +1);
    }

    uint32_t * GetUniqueKmer( size_t index)
    {
        return bytes + index *(max_kmer_count +1 );
    }

    private:

        uint32_t * bytes;
        size_t num_kmer;
        size_t max_kmer_count;
};

/*************************************
 * use 25 bit for a barcode.
 *
 * reduce 20% of memory use.
 * reduce 50% of search speed. 
 *
 * not suggest.
 * only support small endian now.
 ************************************/

struct UniqueKmerArray25Bit
{
    const static uint32_t BarcodeBit = 25;

    const static uint32_t BarcodeMask = (0x1 << BarcodeBit) -1 ;

    const static uint32_t BlockBit = 32;

    UniqueKmerArray25Bit(size_t num_k, size_t max_kcount)
        :bytes(nullptr)
        ,num_kmer(num_k)
        ,max_kmer_count(max_kcount)
        ,barcode_array(0)
    {
        barcode_array = ( max_kmer_count * BarcodeBit ) / BlockBit +1+1; // 1 for count and 1 for (max_kmer_count*BarcodeBit)%sizeof(int)
        size_t total_size =num_kmer * barcode_array;
        bytes = new uint32_t[total_size];
        memset(bytes,0,total_size*sizeof(uint32_t));
        std::cout<<"Alloc size "<<total_size*sizeof(uint32_t)<<" byte"<<std::endl;
    }

    ~UniqueKmerArray25Bit()
    {
        if(bytes)
           delete []bytes;
        bytes = nullptr;
    }

    inline size_t Count(size_t index){
        assert(index < num_kmer && bytes );
        return *(int*)GetUniqueKmer(index);
    }

    inline void Append(size_t index , uint32_t barcodeId)
    {
        assert(index < num_kmer && Count(index) < max_kmer_count );
        uint32_t * k_start = GetUniqueKmer(index);
#ifndef BIG_ENDIAN_FLAG
        uint64_t * b_start =(uint64_t*) (k_start+1+(*k_start)*BarcodeBit/BlockBit) ;
        uint32_t start_bit = (*k_start)*BarcodeBit % BlockBit;
        (*b_start) += (((uint64_t)barcodeId) << start_bit );
#else
        assert(0);
#endif
        (*k_start) ++ ;
    }

    inline int GetBarcodeId(int index , int count)
    {
        uint32_t * k_start = GetUniqueKmer(index);
        assert( count < *k_start);
        k_start ++ ;
#ifndef BIG_ENDIAN_FLAG
        uint64_t * b_start =(uint64_t*) (k_start+(count)*BarcodeBit/BlockBit) ;
        int start_bit = (count)*BarcodeBit % BlockBit;
        return GetBits(*b_start,start_bit+BarcodeBit,BarcodeBit);
#else
        assert(0);
#endif
        return -1;
    }
    private:
        unsigned int * bytes;
        size_t num_kmer;
        size_t max_kmer_count;
        size_t barcode_array;

    private:
        inline unsigned int * GetUniqueKmer(size_t index)
        {
            return bytes + index * barcode_array;
        }

        inline unsigned int GetBits(uint32_t base ,int from , int len)
        {
            return (base >> (from -len)) & ((0x1 <<len) -1);
        }
        inline unsigned int GetBits(uint64_t base ,int from , int len)
        {
            return (base >> (from -len)) & ((0x1UL <<len) -1);
        }

}; //class UniqueKmerArray25Bit

#endif //__LFR_UNIQUEKMERARRAY_H__
