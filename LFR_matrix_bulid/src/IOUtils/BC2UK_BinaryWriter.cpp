#include "BC2UK_BinaryWriter.h"
#include <iostream>
BC2UKWriter_Binary::BC2UKWriter_Binary(const std::string & file_name) :/* prev_id(-1) ,*/fs(nullptr)
{
    fs = fopen(file_name.c_str(),"w");
    if(!fs)
        std::cerr<<"can not open file "<<file_name<<" for write ! ERROR !"<<std::endl;
}

void BC2UKWriter_Binary::write1barcode(int barcode_id,const std::vector<uint32_t> &data)
{
    if(fs&&data.size() >0)
    {
        int buff[2];
        buff[0] = barcode_id ;
        buff[1] = data.size();
        fwrite(buff, sizeof(int),2,fs);
        fwrite(data.data() ,sizeof(uint32_t) , buff[1],fs);
    }
    //.clear();
    //ukdetecter.Clear();
}
/*
void BC2UKWriter_Binary::Append(int barcode_id , uint32_t kmer_id)
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
