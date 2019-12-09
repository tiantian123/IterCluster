#include "Test.h"
#include "KmerHash.h"

TEST_MODULE_INIT(KmerHash);

TEST(KmerIndex_Correct)
{
#if DEBUG
    CHECK((0x1<<18),KMASK+1);
    CHECK(9,KVALUE);
    CHECK(18,KBITS);
#else
    CHECK((0x1UL<<34),KMASK+1);
    CHECK(17,KVALUE);
    CHECK(34,KBITS);
#endif

    CHECK(0xd8 ,(KmerIndex("AGCTA",5,kmerSeq2ID[0]))); // 00 11 01 10 00
    CHECK(0xd8 ,(KmerIndex("TAGCT",5,kmerSeq2ID[0]))); //reverse
}

const int testKValue = 9 ;
const size_t testKMask = (0x1 << 18) -1;
TEST(FullKmerArray_speed_10K)
{
    const int size1 = 10000;// 0x1<<19;
    FullKmerHash k(9);
    char seq[size1+1];
    {
        Timer t("init data");
        for(int i = 0 ; i< size1 ; i++)
        {
            seq[i] ="AGCT" [random() %4 ];
        }
        seq[size1] = 0;
    }
    {
        Timer t("init hash 1");
        for( size_t i = 0 ; i< size1 - testKValue  ; i++)
        {
            k[KmerIndex(seq+i,testKValue,kmerSeq2ID[0])]++;
        }
    }
    std::map<int,int> total;
    for( size_t i = 0 ; i<=testKMask ; i++ )
    {
        total[k[i]]++;
    }
    for(std::map<int,int>::iterator i = total.begin() ; i!= total.end() ; i++)
    {
        if(i->first != 0)
            std::cout<<i->first<<" -- "<<i->second<<std::endl;
    }
}

TEST(FullKmerArray_speed_100K)
{
    const int size1 = 100000;// 0x1<<19;
    FullKmerHash k(9);
    char seq[size1+1];
    {
        Timer t("init data");
        for(int i = 0 ; i< size1 ; i++)
        {
            seq[i] ="AGCT" [random() %4 ];
        }
        seq[size1] = 0;
    }
    {
        Timer t("init hash 2");
        for( size_t i = 0 ; i< size1 - testKValue ; i++)
        {
            k[KmerIndex(seq+i,testKValue,kmerSeq2ID[0])]++;
        }
    }
    std::map<int,int> total;
    for( size_t i = 0 ; i<=testKMask ; i++ )
    {
        total[k[i]]++;
    }
    for(std::map<int,int>::iterator i = total.begin() ; i!= total.end() ; i++)
    {
        if(i->first != 0)
            std::cout<<i->first<<" -- "<<i->second<<std::endl;
    }
}

TEST(FullKmerArray_speed_1M)
{
    const int size1 = 1000000;// 0x1<<19;
    FullKmerHash k(9);
    char seq[size1+1];
    {
        Timer t("init data");
        for(int i = 0 ; i< size1 ; i++)
        {
            seq[i] ="AGCT" [random() %4 ];
        }
        seq[size1] = 0;
    }
    {
        Timer t("init hash 3");
        for( size_t i = 0 ; i< size1 - testKValue ; i++)
        {
            k[KmerIndex(seq+i,testKValue,kmerSeq2ID[0])]++;
        }
    }
    std::map<int,int> total;
    for( size_t i = 0 ; i<=testKMask ; i++ )
    {
        total[k[i]]++;
    }
    for(std::map<int,int>::iterator i = total.begin() ; i!= total.end() ; i++)
    {
        if(i->first != 0)
            std::cout<<i->first<<" -- "<<i->second<<std::endl;
    }
}

/*TEST(KmerValue_test)
{
    CHECK(false,KmerValue().IsSetted());
    CHECK(true,KmerValue("AGCT",4).IsSetted());
    CHECK(0xd8 ,size_t(KmerValue("AGCTA",5))); // 00 11 01 10 00
    CHECK(0xd8 ,size_t(KmerValue("TAGCT",5))); //reverse
    CHECK(0xd8 ,size_t(KmerValue(KmerValue("AGCTA",5))));
    CHECK(true,KmerValue(KmerValue("AGCTA",5)).IsSetted());
    KmerValue t = KmerValue("AGCTA",5);
    CHECK(KmerValue("AGCTA",5), KmerValue("TAGCT",5));
    CHECK(false, (KmerValue("AGTTA",5) == KmerValue("TAGCT",5)));

    CHECK(true,t.IsSetted());
    CHECK(0xd8,size_t(t));
    t.UnSet();
    CHECK(false,  t.IsSetted());
}*/

TEST(KmerHash_correct)
{
    KmerHash k(1000);
    char seq[] = "AGCTGCATGGTAACGCTTCCGTGAATGCGATATGTCGCTAGATAGCTGCATGGTAACGCTTCCGTGAATGCGATATGTCGCTAGAT";
    for( size_t i = 0 ; i< strlen(seq) - 10 ; i++)
    {
        k[KmerIndex(seq+i,10,kmerSeq2ID[0])].value ++;
    }
}

TEST(KmerHash_speed_2M)
{
    const int size1 = 2000000;// 0x1<<19;
    char seq[size1+1];
    {
        Timer t("init data");
        for(int i = 0 ; i< size1 ; i++)
        {
            seq[i] ="AGCT" [random() %4 ];
        }
        seq[size1] = 0;
    }
    KmerHash k(1000000);
    std::map<size_t,int> kk;
    {
        Timer t("init hash 4");
        for( size_t i = 0 ; i< size1 - testKValue; i++)
        {
            k[KmerIndex(seq+i,testKValue,kmerSeq2ID[0])].value ++;
        }
    }
}
