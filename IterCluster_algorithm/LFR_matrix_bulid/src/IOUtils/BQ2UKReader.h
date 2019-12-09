#ifndef __LFR_IOUTILS_BQ2UKREADER_H__
#define __LFR_IOUTILS_BQ2UKREADER_H__
#include <fstream>
#include <vector>
#include "Define.h"
#include "gzstream.h"

struct BC2UKReader
{

    BC2UKReader(const std::string & file_name);

    ~BC2UKReader(){ igs.close();}
    /*{
        if(fd.is_open())
            fd.close();
    }*/

    int get(std::vector<uint32_t> & kmers);

    bool eof;

    private:
        //FILE *  fd;
        //std::ifstream fd;
        igzstream igs; 
};

#endif //__LFR_IOUTILS_BQ2UKREADER_H__
