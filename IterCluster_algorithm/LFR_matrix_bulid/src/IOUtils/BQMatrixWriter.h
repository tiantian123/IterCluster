#ifndef __LFR_UTILS_BQMATRIXWRITER_H__
#define __LFR_UTILS_BQMATRIXWRITER_H__
#include <string>
#include <ArrayHash.h>
#include "Define.h"
#include "gzstream.h"

typedef ArrayHash<int,float>  FactorMap;

struct BCMatrixWriter
{
    BCMatrixWriter(const std::string &name);

 //   void Append(int barcode_id , int barcode_id1 , float factor);
    void Write1Barcode(int barcode_id,int max,FactorMap &itembuffer);

    ~BCMatrixWriter(){
        ogs.close() ;
    }
    /*{
        if(fd)
            fclose(fd);
        fd = nullptr;
    }*/
    private:
        ogzstream ogs;
  //      int prev_id;
   //        FILE * fd;
  //      std::map<int,float> itembuffer;

};

#endif //__LFR_UTILS_BQMATRIXWRITER_H__
