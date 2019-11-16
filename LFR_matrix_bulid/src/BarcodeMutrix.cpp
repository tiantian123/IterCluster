#include "Logic.h"
#include "BQ2UKReader.h"
#include "BC2UK_BinaryReader.h"
#include "BQMatrixWriter.h"
#include "Global.h"
#include "Marco.h"
#include "MultiThread.h"
#include "Timer.h"

static BCMatrixWriter * writer;

static int *barcode_ids;//[BUFFMAX];
static int *max;//[BUFFMAX];
static int *totals;
static std::vector<uint32_t> *kmers;//[BUFFMAX];
static FactorMap *factors;//[BUFFMAX];

void calc1barcode  (int barcode
        , const std::vector<uint32_t> & kmer
        , FactorMap & factor
        , int & max_k){
    ArrayHash<int,int> count(10000);
    size_t len = kmer.size();
    for(size_t i = 0 ; i< len;  i++ )
    {
        uint32_t * bars = The_UniqueKmerArray->GetUniqueKmer(kmer[i]);
        for( uint32_t i = 0 ; i < *bars ; i++ )
        {
            ArrayHash<int,int>::slot *ptr = count.Query(bars[i+1]);
            if(ptr == nullptr)
                count.Insert(bars[i+1],1);
            else
                ptr->value ++ ;
        }
    };
    max_k = count[barcode].value;
    if(max_k < 1 )
        return ;
    for( int i = 0 ; i< count.size; i++)
    {
        ArrayHash<int,int>::slot & t = count.slot_list[i];
        if( t.key != ArrayHash<int,int>::INVALID_KEY)
        {
            factor[t.key ].value = float(t.value) / float(max_k) ;
        }
    }
}

static int index_1 = 0;
static int tall;

void multi_calcmatrix(int t_num) {
    for( int i = 0 ; i<index_1; i++)
    {
        if( i % tall== t_num )
        {
            calc1barcode(barcode_ids[i],kmers[i] ,factors[i],max[i]);
            kmers[i].clear();
        }
    }
};

void print_matrx()
{
    for( int i = 0 ; i< index_1; i++ )
    {
        writer->Write1Barcode(barcode_ids[i],max[i],factors[i]);
        factors[i].Clear();
        barcode_ids[i] = -1;
    }
};
void BarcodeMutrix()/*const std::string &inf,
        const std::string &outf,
        int thread_num)*/
{
    ///////////  Variable define ////////////////////////////
    Timer t("Construct barcode matrix and write file");
    tall = ThreadNum;
    writer = new BCMatrixWriter(file_barcode_matrix);
    {
        Timer("Init buffer");
        barcode_ids =  new int[BUFFMAX];
        max = new int[BUFFMAX];
        kmers = new std::vector<uint32_t>[BUFFMAX];
        factors = new FactorMap[BUFFMAX];

        for(int i = 0 ; i < BUFFMAX ; i++)
        {
            barcode_ids[i] = -1;
        }
    }
    ///////////  Function define ////////////////////////////


    ///////////  Logic flow ////////////////////////////
    MultiThread tControl;
    tControl.Init(ThreadNum);
    tControl.RegisterSignal(1,multi_calcmatrix);
    tControl.Run();

#define READ_ALL \
        while(true)\
        {\
            barcode_ids[index_1] = reader.get(kmers[index_1]);\
            if(reader.eof)\
                break;\
            index_1 ++ ;\
            if(index_1 == BUFFMAX)\
            {\
                tControl.SendWorkSignal(1);\
                print_matrx();\
                index_1 = 0;\
            }\
        }

    if(!BinaryOutput)
    {
        BC2UKReader reader(file_barcode2kmer);
        READ_ALL;
    }
    else
    {
        BC2UKReader_Binary reader(file_barcode2kmer);
        READ_ALL;
    }

    if(index_1 > 0 )
    {
        tControl.SendWorkSignal(1);
        print_matrx();
    }
    tControl.Quit();
    delete []   barcode_ids;
    delete []   max ;
    delete []   kmers;
    delete []   factors;
    delete writer;
    writer = NULL;
}

void LoadKmer2BarcodeHash()
{
    std::vector<uint32_t> buffer;
    if( !BinaryOutput)
    {
        BC2UKReader reader(file_barcode2kmer);
        while(true)
        {
            int barcode = reader.get(buffer);
            if(reader.eof)
                break;
            for(size_t i = 0 ; i < buffer.size() ; i++)
            {
                The_UniqueKmerArray->Append(buffer[i],barcode);
            }
            buffer.clear();
        }
    }
    else
    {
        BC2UKReader_Binary reader(file_barcode2kmer);
        while(true)
        {
            int barcode = reader.get(buffer);
            if(reader.eof)
                break;
            for(size_t i = 0 ; i < buffer.size() ; i++)
            {
                The_UniqueKmerArray->Append(buffer[i],barcode);
            }
            buffer.clear();
        }
    }
}
