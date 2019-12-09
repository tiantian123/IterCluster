#include "KmerHash.h"
#include "SeqReader.h"
#include "Logic.h"
#include "MultiThread.h"
#include "Global.h"
#include "Timer.h"
#include "Read2KmerBuffer.h"


static    Read2KmerBuffer buffer;
static    int index_1 = 0 ;

void IncrKmerCount ()
{
    if( UseFullKmerArray )
    {

        for(int i = 0 ; i < index_1; i++ )
        {
            if(buffer.seqs[i].barcode != Read2KmerBuffer::BarcodeFlag01)
                break;
            uint64_t * kmer = buffer.kmers[i];
            for( int j = 0 ; j< buffer.kmerlen ; j++)
            {
                (*The_FullKmerHash)[kmer[j]] ++ ;
            }
            buffer.seqs[i].barcode = Read2KmerBuffer::BarcodeFlag02;
        }
    }
    else
    {
        for(int i = 0 ; i < index_1; i++ )
        {
            if(buffer.seqs[i].barcode != Read2KmerBuffer::BarcodeFlag01)
                break;
            uint64_t * kmer = buffer.kmers[i];
            for( int j = 0 ; j< buffer.kmerlen ; j++)
            {
                (*The_KmerHash)[kmer[j]].value++;
            }
            buffer.seqs[i].barcode = Read2KmerBuffer::BarcodeFlag02;
        }
    }
};
static int tall;

void chopReads2Kmer ( int t_num){
    for( int i = 0 ; i < index_1; i++ )
    {
        if( i%tall== t_num)
        {
            buffer.chopRead2Kmer(i);
            buffer.seqs[i].barcode = Read2KmerBuffer::BarcodeFlag01;
        }
    }
};

void Read2KmerCount(const std::string & file_name , int thread_num)
{
    tall = thread_num;
    //////////////////// Variable define //////////////////
    Timer t("Read2KmerCount for file "+file_name);
    MultiThread tControl;
    SeqReader reader(file_name);
    SeqRecord record;

    //////////////////// Function define //////////////////


    //////////////////// Logic flow ////////////////////////
    tControl.Init(thread_num);
    tControl.RegisterSignal(1, chopReads2Kmer);
    reader.get(record);
    int kmerlen = record.seq.size() - KValue+1;
    if( kmerlen <= 0 )
    {
        std::cerr<<" KValue greater than read len !"<<std::endl;
        abort();
    }
    buffer.Init(kmerlen);
    buffer.seqs[0] = record;
    index_1 ++ ;
    tControl.Run();
    while( reader.get(buffer.seqs[index_1]))
    {
        index_1 ++ ;
        if(index_1 == BUFFMAX)
        {
            tControl.SendWorkSignal(1);
            IncrKmerCount();
            index_1 = 0;
        }
    }

    if(index_1 > 1)
    {
        tControl.SendWorkSignal(1);
        IncrKmerCount();
    }

    tControl.Quit();
}
static FILE * fd1;
/*
static ArrayHash<long,long>  kmerAnlysis;

#define ADD_COUNT \
    ArrayHash<long,long>::slot* p =  kmerAnlysis.Query(count);\
    if( p != NULL )\
    {\
        p->value += 1;\
    }\
    else\
    {\
        kmerAnlysis.Insert(count,1);\
    }
*/

static void printKmerCount(KmerHash::slot & item)
{
    int count = item.value;
    if( count > 0 )
    {
        fprintf(fd1,"%lu %d\n",item.key,count);
        //ADD_COUNT;
    }
}
static void printKmerAnlysis(ArrayHash<long,long>::slot & item)
{
    int count = item.value;
    if( count > 0 )
    {
        fprintf(fd1,"%lu %d\n",item.key,count);
    }
}

void printHash(const std::string & name)
{
    FILE * fd = fopen(name.c_str(),"w");
    if( fd == NULL )
    {
        std::cerr<<"ERROR : can not open "<<name<<" for write !"<<std::endl;
        exit(0);
    }

    if(UseFullKmerArray)
    {
        for(long i = 0 ; i< The_FullKmerHash->size ; i++)
        {
            long count = (*The_FullKmerHash)[i];
            if( count > 0 )
            {
                fprintf(fd,"%lu %lu\n",i,count);
     //           ADD_COUNT;
            }
        }
    }
    else
    {
        fd1 = fd ;
        The_KmerHash->Foreach(printKmerCount);
    }
    fclose(fd);
    //fd = fopen(file_kmer_anlysis.c_str() , "w");
    //if(fd == NULL)
    //{
    //    std::cerr<<"ERROR : can not open "<<file_kmer_anlysis<<" for write !"<<std::endl;
    //    exit(0);
    //}
    //fd1 = fd ;
    //kmerAnlysis.Foreach(printKmerAnlysis);
    //fclose(fd);
    fd1 = NULL;
}

void printKmerSeq2ID()
{
	std::fstream fd;
	fd.open("kmerID2KmerSeq.txt",std::fstream::out);
	if(fd.is_open())
	{
		std::cerr << "ERROR: cannot open kmerSeq2kmerID.txt " << std::endl;
		exit(0);
	}
	for(std::vector < std::map< uint64_t, std::string > > :: iterator iter = kmerSeq2ID.begin(); iter != kmerSeq2ID.end(); iter++)
	{
		for(std::map< uint64_t, std::string> :: iterator iter2 = iter->begin(); iter2 != iter->end(); iter2++)
		{
			fd << iter2->first << "\t" << iter2->second;
		}
	}
}

void PrintKmerCount()
{
    printHash(file_kmer_count);
	printKmerSeq2ID();
}

void loadHash(const std::string & name)
{
    FILE * fd  = fopen(name.c_str(),"r");
    if( fd == NULL )
    {
        std::cerr<<"ERROR : can not open "<<name<<" for read!"<<std::endl;
        exit(0);
    }
    long kmer = 0 , count = 0;

    if( UseFullKmerArray)
    {
        while(fscanf(fd,"%lu %lu\n",&kmer, & count) == 2)
        {
            (*The_FullKmerHash)[kmer] = count;
        }
    }
    else
    {
        while(fscanf(fd,"%lu %lu\n",&kmer, & count) == 2)
        {
            The_KmerHash->Insert(kmer,count);
        }
    }
}
void LoadKmerCount()
{
    loadHash(file_kmer_count);
}
