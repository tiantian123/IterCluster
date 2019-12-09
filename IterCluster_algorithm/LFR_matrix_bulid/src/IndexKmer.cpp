#include "Global.h"
#include "Logic.h"
#include "Timer.h"
#include "Define.h"
#if DEBUG
int unique_kmer_num = 0;
int total_unique = 0;
int repeat_kmer_num = 0;
int total_repeat = 0;
#endif

static uint64_t index_1 , min , max;

void index_1_hash(KmerHash::slot & item){
    if(item.key == KmerHash::INVALID_KEY)
        return ;
    if(item.value < min|| item.value > max)
    {
#if DEBUG
        repeat_kmer_num ++ ;
        total_repeat += item.value;
#endif
        item.value = 0;
        return ;
    }
#if DEBUG
    unique_kmer_num ++ ;
    total_unique += item.value;
#endif
    item.value = ++index_1;
};

uint64_t IndexKmers(int i,int a)
{
    min = i;
    max = a;
    Timer t("Index unique kmer");

    if(UseFullKmerArray)
    {
        for(FullKmerHash::KeyType i = 0 ; i <The_FullKmerHash->size ; i++)
        {
            if( (*The_FullKmerHash)[i] < (FullKmerHash::KeyType)min
                    || (*The_FullKmerHash)[i] > (FullKmerHash::KeyType)max )
            {
                (*The_FullKmerHash)[i] = 0;
            }
            else
            {
                (*The_FullKmerHash)[i] = ++index_1;
            }
        }
    }
    else
    {
        The_KmerHash->Foreach(index_1_hash);
#if DEBUG
        std::cerr<<"Unique kmer number "<<unique_kmer_num<<std::endl;
        std::cerr<<"repeqt kmer number "<<repeat_kmer_num<<std::endl;
        std::cerr<<"Total Unique kmer number "<<total_unique<<std::endl;
        std::cerr<<"Total retpeat kmer number "<<total_repeat<<std::endl;
#endif
    }
    std::cout<<"IndexKmers -- total unique kmer : "<<index_1<<std::endl;
    return index_1;
}


void PrintUniqueKmer()
{
    printHash(file_uniquekmer_index);
}
void LoadUniqueKmer()
{
    loadHash(file_uniquekmer_index);
}
