#ifndef __LFR_IOUTILS_BQ2UKWRITER_BINARY_H__
#define __LFR_IOUTILS_BQ2UKWRITER_BINARY_H__

#include <string>
#include <stdint.h>
#include <vector>
#include "ArrayHash.h"
#include "Define.h"
struct BC2UKWriter_Binary
{
    BC2UKWriter_Binary(const std::string & file_name);

    //void Append(int barcode_id , uint32_t kmer_id);

    ~BC2UKWriter_Binary()
    {
       /* if(kmerbuffer.size() > 1)
            write1barcode(kmerbuffer);*/
        if(fs)
            fclose(fs);
        fs = nullptr;
    }
    private:
        //int prev_id;
        FILE *fs;
        //ArrayHash<int32_t,int> ukdetecter;
        //std::vector<uint32_t> kmerbuffer;
    //private:
    public:
        void write1barcode(int barcode_id ,const std::vector<uint32_t> & data);
};

#endif //__LFR_IOUTILS_BQ2UKWRITER_BINARY_H__
