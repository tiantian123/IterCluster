#ifndef __LFR_IOUTILS_BQ2UKREADER_BINARY_H__
#define __LFR_IOUTILS_BQ2UKREADER_BINARY_H__
#include <fstream>
#include <vector>
#include "Define.h"
struct BC2UKReader_Binary
{

    BC2UKReader_Binary(const std::string & file_name);

    ~BC2UKReader_Binary()
    {
        if(fd)
            fclose(fd);
        fd = nullptr;
    }

    int get(std::vector<uint32_t> & kmers);

    bool eof;

    private:
        FILE *  fd;
};

#endif //__LFR_IOUTILS_BQ2UKREADER_BINARY_H__
