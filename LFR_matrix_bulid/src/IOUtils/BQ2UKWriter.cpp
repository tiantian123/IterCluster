#include "BQ2UKWriter.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
BC2UKWriter::BC2UKWriter(const std::string & file_name) : ogs(file_name.c_str()) {} 
/*prev_id(-1) ,fs(nullptr)*/
/*{
    fs = fopen(file_name.c_str(),"w");
    if(!fs)
        std::cerr<<"can not open file "<<file_name<<" for write ! ERROR !"<<std::endl;
}*/

void BC2UKWriter::write1barcode(int barcode_id , const std::vector<uint32_t> & data)
{
    size_t size = data.size() ;
    if( size < 1 )
        return;
    ogs<<barcode_id<<" "<<size;
    for(size_t i = 0 ; i< size ; i++ )
        ogs<<" "<<data[i];
    ogs<<std::endl;

    /*if(fs && data.size() > 0)
    {
            int buff[2];
        buff[0] = barcode_id;
        buff[1] = data.size();
        fprintf(fs,"%d %d",buff[0], buff[1]);
        for( int i = 0 ; i < buff[1] ; i ++ )
            fprintf(fs," %d",data[i]);
        fprintf(fs,"\n");
    }*/
    //kmerbuffer.clear();
    //ukdetecter.Clear();
}
/*
void BC2UKWriter::Append(int barcode_id , uint32_t kmer_id)
{
    if(prev_id == -1)
    {
        prev_id = barcode_id;
    }
    if(barcode_id != prev_id)
    {
        write1barcode();
        prev_id = barcode_id;
    }
    if(ukdetecter.Query(kmer_id) != nullptr)
        return ;
    ukdetecter.Insert(kmer_id,1);
    kmerbuffer.push_back(kmer_id);
};*/
