#include "Logic.h"
#include "LFRReader.h"
#include "MultiThread.h"
#include "Marco.h"
#include "Read2KmerBuffer.h"
#include "BQ2UKWriter.h"
#include "BC2UK_BinaryWriter.h"
#include "Global.h"
#include "Timer.h"

static Read2KmerBuffer buffer;
static int index_1 = 0 ;

static BC2UKWriter * writer = NULL;
static BC2UKWriter_Binary * writer_b = NULL;

void dealKmers(FILE * fd)
{
    static ArrayHash<int32_t,int> utotal;
    static ArrayHash<int32_t,int> ukdetecter;
    static std::vector<uint32_t> data;
    static int prev_id = -1;
    if(prev_id == -1)
        prev_id = buffer.seqs[0].barcode;

#define HANLE_PREV \
    int barcode_id = buffer.seqs[i].barcode;\
    if( prev_id != barcode_id )\
    {\
        if(BinaryOutput)\
        writer_b->write1barcode(prev_id,data);\
        else\
        writer->write1barcode(prev_id,data);\
        data.clear();\
        int uk = ukdetecter.Clear();\
        int all = utotal.Clear();\
        fprintf(fd,"%d\t%d\t%d\n",prev_id,uk,all);\
        prev_id = barcode_id;\
    }

#define HANLE_INDEX \
    if( index_1 < 1)\
    continue;\
    if(ukdetecter.Query(index_1-1) == nullptr)\
    {\
        ukdetecter.Insert(index_1-1, 1);\
        data.push_back(index_1-1);\
    }\

    if(UseFullKmerArray)
    {
        for(int i = 0 ; i < index_1; i++ )
        {
            HANLE_PREV;
            uint64_t * kmer = buffer.kmers[i];
            for( int j = 0 ; j< buffer.kmerlen ; j++)
            {
                if(utotal.Query(kmer[j]) == NULL)
                {
                    utotal.Insert(kmer[j],1);
                }
                uint64_t index_1 = 0;
                index_1 = (*The_FullKmerHash)[kmer[j]];
                HANLE_INDEX;
            }
            buffer.seqs[i].barcode = Read2KmerBuffer::BarcodeFlag02;
        }
    }
    else
    {
        for(int i = 0 ; i < index_1; i++ )
        {
            HANLE_PREV;
            uint64_t * kmer = buffer.kmers[i];
            for( int j = 0 ; j< buffer.kmerlen ; j++)
            {
                if(utotal.Query(kmer[j]) == NULL)
                {
                    utotal.Insert(kmer[j],1);
                }
                uint64_t index_1 = 0;
                KmerHash::slot * curr= The_KmerHash->Query(kmer[j]);
                if(curr == nullptr)
                    continue;
                index_1 = curr->value;
                HANLE_INDEX;
            }
            buffer.seqs[i].barcode = Read2KmerBuffer::BarcodeFlag02;
        }
    }
};

static int tall;
void chopRead2Kmer(int t_num){ 
    for( int i = 0 ; i < index_1; i++ )
    {
        if( i%tall== t_num)
        {
            buffer.chopRead2Kmer(i);
        }
    }
};

void Kmer2BarcodeHash()
    /*const std::string &f1, 
        const std::string &f2,
        int thread_num,
        const std::string &outf)*/
{
    ////////////// Variable define ///////////////////////
    Timer t("Construct Kmer2BarcodeHash and write Barcode2Kmer file ");
    tall = ThreadNum;
    MultiThread tControl;
    SeqReader *reader_single = NULL;
    LFRPEReader *reader_pe = NULL;
    if( file2.empty() )
        reader_single= new SeqReader(file1);
    else
        reader_pe = new LFRPEReader(file1,file2);

    SeqRecord record;
    if(!BinaryOutput)
        writer = new BC2UKWriter(file_barcode2kmer);
    else
        writer_b = new BC2UKWriter_Binary(file_barcode2kmer);

    ////////////// Function define ///////////////////////


    /////////////////// Logic flow ///////////////////
    tControl.Init(ThreadNum);
    tControl.RegisterSignal(1,chopRead2Kmer);
    if( reader_single )
        reader_single->get(record);
    else
        reader_pe->get(record);

    int kmerlen = record.seq.size() - KValue +1;
    if( kmerlen <= 0 )
    {
        std::cerr<<" KValue greater than read len !"<<std::endl;
        abort();
    }
    buffer.Init(kmerlen);
    buffer.seqs[0] = record;
    index_1 ++;
    tControl.Run();
    FILE * fd = fopen(file_barcode_info.c_str(),"w");
    if(fd == NULL)
    {
        std::cerr<<"ERROR : can not open "<<file_barcode_info<<" for writer"<<std::endl;
        exit(0);
    }

    if( reader_single )
    {
        while( reader_single->get(buffer.seqs[index_1]))
        {
            index_1 ++;
            if(index_1 == BUFFMAX)
            {
                tControl.SendWorkSignal(1);
                dealKmers(fd);
                index_1 = 0;
            }
        }
    }
    else
    {
        while( reader_pe->get(buffer.seqs[index_1]))
        {
            index_1 ++;
            if(index_1 == BUFFMAX)
            {
                tControl.SendWorkSignal(1);
                dealKmers(fd);
                index_1 = 0;
            }
        }
    }

    if(index_1 > 1)
    {
        tControl.SendWorkSignal(1);
        dealKmers(fd);
    }

    tControl.Quit();
    if(writer)
        delete writer;
    if(writer_b)
        delete writer_b;
    fclose(fd);
}
