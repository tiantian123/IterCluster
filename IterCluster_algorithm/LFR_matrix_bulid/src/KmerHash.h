#ifndef __LFR_KMERHASH_H__
#define __LFR_KMERHASH_H__

#include <stdint.h>
#include <string>
#include "ArrayHash.h"
#include "Marco.h"

template<class KEY_TYPE>
struct FullArray
{
    typedef KEY_TYPE KeyType;

    const  KEY_TYPE size;

    FullArray(int k) :array(nullptr) ,size(0x1UL<< (2*k))
    {
        array = new KEY_TYPE[size];
        memset(array,0,sizeof(KEY_TYPE) * size);
    };

    ~FullArray()
    {
        if(array)
            delete [] array;
        array = nullptr;
    }

    KEY_TYPE & operator[]( KEY_TYPE key )
    {
        return array[key];
    }

    private:
        KEY_TYPE * array;
};

typedef FullArray<int64_t> FullKmerHash;

typedef ArrayHash<int64_t , int32_t> KmerHash;



#endif //__LFR_KMERHASH_H__
