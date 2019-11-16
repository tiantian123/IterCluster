#include "BQ2UKReader.h"
#include <iostream>
//#include <String.h>
#include <cassert>
#include <sstream>

BC2UKReader::BC2UKReader(const std::string & file_name ) : igs(file_name.c_str()){}
    /*eof(false),fd(nullptr)
{
    fd.open(file_name.c_str());
}*/

int BC2UKReader::get(std::vector<uint32_t> & kmers)
{
    std::string line ;
    int id;
    int count;
    eof = !(getline(igs,line));
    if( eof )
        return -1;
    else
    {
        std::istringstream iss(line);
        iss>>id;
        iss>>count;
        for(int i = 0 ; i < count ; i++)
        {
            int n ;
            iss>>n;
            kmers.push_back(n);
        }
    }
    return id;
}
