#include <iostream>
#include "Global.h"
#include "Logic.h"
#include <unistd.h>
#include "Timer.h"
#include <sstream>
#include <fstream>
namespace option
{

    static long long hash_init_size = 0x1UL << 25;

#define OPTION_CHECK_O \
        if(! flag )\
        {\
            std::cerr<<"ERROR : you must assign a ouput file prefix by -o option!"<<std::endl;\
            return false ;\
        }

#define OPTION_CHECK_U\
        if( KIndexMax<1)\
        {\
            std::cerr<<"ERROR : Invalid KIndexMax" <<KIndexMax<<" !"<<std::endl;\
            return false;\
        }
#define OPTION_CHECK_K\
        if( KValue % 2 == 0)\
        {\
            std::cerr<<"ERROR : KValue is odd!"<<std::endl;\
            return false;\
        }

#define OPTION_CHECK_T \
        if( ThreadNum < 1)\
        {\
            std::cerr<<"ERROR : ThreadNum invalid by "<<ThreadNum<<std::endl;\
            return false;\
        }

#define OPTION_CHECK_GAP \
        if(KCountMin < 0 || KCountMin > KCountMax ) \
        {\
            std::cerr<<"ERROR : unique kmer area invalid by [ "<<KCountMin <<" , "<<KCountMax <<" ]"<<std::endl;\
            return false ;\
        }

#define DEAL_OPTIONS_START \
        int opt ;\
        bool flag = false ;\
        while((opt = getopt(argc,argv, "u:o:i:a:k:t:p:e:z:f")) != EOF)\
        {\
            char fprefix[1024];\
            switch(opt)\
            {

#define DEAL_OPTIONS_END \
                default:\
                    break;\
            }\
        }

#define DEAL_PE \
                case 'p':\
                    sscanf(optarg,"%s",fprefix); \
                    file1 = std::string(fprefix);\
                    break;\
                case 'e':\
                    sscanf(optarg,"%s",fprefix);\
                    file2 = std::string(fprefix);\
                    break;

#define DEAL_O \
                case 'o':\
                    sscanf(optarg,"%s",fprefix); \
                    file_barcode2kmer = std::string(fprefix) + ".barcode2kmer.gz";\
                    file_barcode_matrix = std::string(fprefix)+".barcode_matrix.gz";\
                    file_uniquekmer_index = std::string(fprefix)+".uniquekmer_index";\
                    file_kmer_count = std::string(fprefix)+".kmer_count";\
                    file_barcode_info = std::string(fprefix)+".barcode_info";\
                    file_kmer_anlysis = std::string(fprefix)+".kmer_anlysis";\
                    flag = true ;\
                    break;

#define DEAL_K \
                case 'k':\
                    sscanf(optarg,"%d",&KValue);\
                    break;\

#define DEAL_U \
                case 'u':\
                    sscanf(optarg,"%lu",&KIndexMax);\
                    break;\

#define DEAL_FZ \
                case 'f':\
                    UseFullKmerArray = true;\
                    break;\
                case 'z':\
                    {\
                        unsigned int size ;\
                        sscanf(optarg,"%ud",&size);\
                        hash_init_size= ((long long)size) * sizeof(KmerHash::slot);\
                    }\
                    break;

#define DEAL_IA \
                case 'i':                           \
                    sscanf(optarg,"%d",&KCountMin); \
                    break;                          \
                case 'a':                           \
                    sscanf(optarg,"%d",&KCountMax); \
                    break;
#define DEAL_T \
                case 't':\
                    sscanf(optarg,"%d",&ThreadNum);\
                    break;

    bool parseOptionsKmerCount(int argc,char **argv)
    {
        DEAL_OPTIONS_START
        DEAL_O
        DEAL_K
        DEAL_FZ
        DEAL_PE
        DEAL_T
        DEAL_OPTIONS_END

        OPTION_CHECK_O
        OPTION_CHECK_K
        OPTION_CHECK_T

        return true;
    }

    bool parseOptionsUniqueKmer(int argc,char **argv)
    {
        DEAL_OPTIONS_START
        DEAL_O
        DEAL_IA
        DEAL_FZ
        DEAL_OPTIONS_END

        OPTION_CHECK_O
        OPTION_CHECK_GAP
        return true;
    }

    bool parseOptionsBarcode2Kmer(int argc,char **argv)
    {
        DEAL_OPTIONS_START
        DEAL_O
        DEAL_FZ
        DEAL_K
        DEAL_T
        DEAL_PE
        DEAL_OPTIONS_END

        OPTION_CHECK_O
        OPTION_CHECK_K
        OPTION_CHECK_T

        return true;
    }


    bool parseOptionsBarcodeMatrix(int argc,char **argv)
    {
        DEAL_OPTIONS_START
        DEAL_O
        DEAL_T
        DEAL_U
        DEAL_IA
        DEAL_OPTIONS_END

        OPTION_CHECK_O
        OPTION_CHECK_U
        OPTION_CHECK_T
        OPTION_CHECK_GAP
        return true;
    }

    bool parseOptions(int argc,char **argv)
    {
        DEAL_OPTIONS_START
        DEAL_O
        DEAL_IA
        DEAL_FZ
        DEAL_K
        DEAL_T
        DEAL_U
        DEAL_PE
        DEAL_OPTIONS_END

        OPTION_CHECK_K
        OPTION_CHECK_O
        OPTION_CHECK_T
        OPTION_CHECK_GAP

        return true;
    }

    void printUsageKmerCount()
    {
        std::cout<<"LFR-martix-build KmerCount      -o output -k 17 -p read1 [-e read2] [-t 8] [-f/-z 1000000]"<<std::endl;
    }

    void printUsageUniqueKmer()
    {
        std::cout<<"LFR-martix-build UniqueKmer     -o output -i min -a max [-t 8] [-f/-z 1000000]"<<std::endl;
    }

    void printUsageBarcode2Kmer()
    {
        std::cout<<"LFR-martix-build Barcode2Kmer   -o output  -k 17 -p read1 [-e read2] [-t 8] [-f/-z 1000000]"<<std::endl;
    }

    void printUsageBarcodeMatrix()
    {
        std::cout<<"LFR-martix-build Barcode2Kmer   -o output -i min -a max -u unique_kmer_num  [-t 8] "<<std::endl;
    }
    void printUsage()
    {
        std::cout<<"LFR-martix-build [KmerCount/UniqueKmer/Barcode2Kmer/BarcodeMatrix/All] [args]"<<std::endl
            <<"args:\n"
            "\n"
            "    -p first fastq file.\n"
            "    -e second fastq file.\n"
            "    -o output filename prefix.\n"
            "    -i min k frequey.\n"
            "    -a max k frequry.\n"
            "    -k [option] k value. default is 17. \n"
            "    -t [option] cpu number. default is 8.\n"
            "    -f [option] set means use Array instead of Hash.\n"
            "    -z [option] kmer number ,used for init hash table, default is 10000000\n";
    }
}

inline void InitHash()
{
    if(UseFullKmerArray)
        The_FullKmerHash = new FullKmerHash(KValue);
    else
    {
        The_KmerHash = new KmerHash(option::hash_init_size);
    }
}
inline void FreeHash()
{
    if(UseFullKmerArray)
    {
        delete The_FullKmerHash;
        The_FullKmerHash = nullptr;
    }
    else
    {
        delete The_KmerHash;
        The_KmerHash = nullptr;
    }
}

inline void LoadRead2KmerCount()
{
    Read2KmerCount(file1,ThreadNum);
    if( ! file2.empty() )
    {
        Read2KmerCount(file2,ThreadNum);
    }
}

int main(int argc , char** argv)
{
    Timer t("main loop");
    if( argc < 2 )
    {
        option::printUsage();
        return 0;
    }
    if(std::string(argv[1]) == "All")
    {
        if( !option::parseOptions(argc,argv))
        {
            option::printUsage();
            return 0;
        }
        else
        {
            //Step 1
            InitHash();
            LoadRead2KmerCount();
            PrintKmerCount();
            //Step 2
            KIndexMax = IndexKmers(KCountMin,KCountMax);
            PrintUniqueKmer();
            //Step 3
            Kmer2BarcodeHash();
            FreeHash();
            //Step 4
            The_UniqueKmerArray = new UniqueKmerArray(KIndexMax, KCountMax);
            LoadKmer2BarcodeHash();
            BarcodeMutrix();
            delete The_UniqueKmerArray;
        }
        return 0;
    }

    if(std::string(argv[1]) == "KmerCount")
    {
        if(!option::parseOptionsKmerCount(argc,argv))
        {
            option::printUsageKmerCount();
            return 0;
        }
        else
        {
            //Step 1
            InitHash();
            LoadRead2KmerCount();
            PrintKmerCount();
            FreeHash();
        }
        return 0;
    }

    if(std::string(argv[1]) == "UniqueKmer")
    {
        if(!option::parseOptionsUniqueKmer(argc,argv))
        {
            option::printUsageUniqueKmer();
            return 0;
        }
        else
        {
            //Step 2
            InitHash();
            LoadKmerCount();
            KIndexMax = IndexKmers(KCountMin,KCountMax);
            std::cout<<"Total unique kmer "<<KIndexMax<<std::endl;
            PrintUniqueKmer();
            FreeHash();
        }
        return 0;
    }

    if(std::string(argv[1]) == "Barcode2Kmer")
    {
        if(!option::parseOptionsBarcode2Kmer(argc,argv))
        {
            option::printUsageBarcode2Kmer();
            return 0;
        }
        else
        {
            //Step3
            InitHash();
            LoadUniqueKmer();
            Kmer2BarcodeHash();
            FreeHash();
        }
        return 0;
    }

    if(std::string(argv[1]) == "BarcodeMatrix")
    {
        if(!option::parseOptionsBarcodeMatrix(argc,argv))
        {
            option::printUsageBarcodeMatrix();
            return 0;
        }
        else
        {
            The_UniqueKmerArray = new UniqueKmerArray(KIndexMax, KCountMax);
            LoadKmer2BarcodeHash();
            BarcodeMutrix();
            delete The_UniqueKmerArray;
        }
        return 0;
    }

    option::printUsage();
    return 0;
}
