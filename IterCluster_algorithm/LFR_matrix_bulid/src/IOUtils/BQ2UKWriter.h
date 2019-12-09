#ifndef __LFR_IOUTILS_BQ2UKWRITER_H__
#define __LFR_IOUTILS_BQ2UKWRITER_H__

#include <string>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include "ArrayHash.h"
#include "Define.h"
#include "gzstream.h"

struct BC2UKWriter
{
    BC2UKWriter(const std::string & file_name);

//    void Append(int barcode_id , uint32_t kmer_id);

    ~BC2UKWriter()
    {
        ogs.close();
  //      if(kmerbuffer.size() > 1)
  //          write1barcode();
        //if(fs)
        //    fclose(fs);
        //fs = nullptr;
    }
    private:
    //    int prev_id;
    //    FILE *fs;
    //    ArrayHash<int32_t,int> ukdetecter;
    //    std::vector<uint32_t> kmerbuffer;
    //private:
          ogzstream ogs;
    public:
        void write1barcode(int barcode_id ,const std::vector<uint32_t> & data);
};

#endif //__LFR_IOUTILS_BQ2UKWRITER_H__
