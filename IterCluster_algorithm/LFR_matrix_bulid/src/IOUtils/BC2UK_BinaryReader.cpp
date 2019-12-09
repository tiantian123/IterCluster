#include "BC2UK_BinaryReader.h"
#include <iostream>
//#include <String.h>
#include <cassert>

BC2UKReader_Binary::BC2UKReader_Binary(const std::string & file_name) : eof(false),fd(nullptr)
{
    fd = fopen(file_name.c_str(),"r");
    if(!fd)
        std::cerr<<" can not open file "<<file_name<<" for read !"<<std::endl;
}

int BC2UKReader_Binary::get(std::vector<uint32_t> & kmers)
{
    int buff[2];
    int got  = fread(buff,sizeof(int),2,fd);
    if( got == 0 )
    {
        eof = true ;
        return -1;
    }
    kmers.clear();
    kmers.resize(buff[1]);
    assert(buff[1] > 0 );

    int left = buff[1];
    while(left > 0 )
    {
        got = fread(kmers.data() + (buff[1] - left) , sizeof(uint32_t) , left,fd);
        if(got == 0 )
        {
            eof = true;
            buff[1] = buff[1] - left;
            return buff[0];
        }
        left -= got ;
        assert( left > -1 );
    };
    return buff[0];
}
