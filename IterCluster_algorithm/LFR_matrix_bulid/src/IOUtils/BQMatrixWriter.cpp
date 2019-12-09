#include "BQMatrixWriter.h"
#include <iostream>
#include <iomanip>

BCMatrixWriter::BCMatrixWriter(const std::string & name) : ogs(name.c_str()) 
{
    ogs<<std::fixed<<std::setprecision(6);
}
/*    fd(nullptr)
{
    fd = fopen(name.c_str(),"w");
    if(!fd)
        std::cerr<<"can not open file "<<name<<" to write . ERROR ."<<std::endl;
}*/
/*
void BCMatrixWriter::Append(int barcode_id, int barcode_id1, float c)
{
    if(prev_id == -1)
        prev_id = barcode_id;
    if(prev_id != barcode_id)
    {
        write1barcode();
        prev_id = barcode_id;
    }
    itembuffer[barcode_id1] = c ;
}*/
static ogzstream * the_fd;
//static FILE * the_fd;
static void printInfo(FactorMap::slot & item)
{
    (*the_fd)<<' '<<item.key<<':'<<item.value;
}

void BCMatrixWriter::Write1Barcode(int barcode_id ,int max ,FactorMap &itembuffer)
{
    //the_fd = fd;
    the_fd = &ogs;
    if( max >0 )
    {
        //the_fd = &ogs;
        ogs<<barcode_id<<':'<<max;
        //fprintf(fd,"%d:%d",barcode_id,max);
        /*itembuffer.Foreach([this](FactorMap::slot & item)
        {
            fprintf(fd," %d:%6f",item.key, item.value);
        });*/
        itembuffer.Foreach(printInfo);
        //fprintf(fd,"\n");
        ogs<<std::endl;
    }
}
