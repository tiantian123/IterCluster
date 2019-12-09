#include "Test.h"
#include "BQ2UKReader.h"
#include "BQ2UKWriter.h"

#include "BC2UK_BinaryReader.h"
#include "BC2UK_BinaryWriter.h"
#include <set>

TEST_MODULE_INIT(BQ2UKWRTest)

#define TEST_FILE  "bc2uk_test"
TEST(BQ2UKRWriter)
{

    std::map<int, std::vector<uint32_t> > expect;
    {
        BC2UKWriter writer(TEST_FILE"1");
        for( int i = 0 ; i < 100 ; i++)
        {
            std::set<int> unique;
            int num = random() % 10000;
            if( num < 1)
                continue;
            for( int j = 0 ; j < num ; j++ )
            {
                int key = random();
                if(unique.find(key) != unique.end())
                    continue;
                unique.insert(key);
                expect[i].push_back(key);

                //writer.Append(i,key);
            }
            writer.write1barcode(i,expect[i]);
        }
    }
    {
        BC2UKReader read(TEST_FILE"1");
        while(true)
        {
            std::vector<uint32_t> buff;
            int id = read.get(buff);
            if( id < 0 )
                break;
            CHECK(true , (buff == expect[id]) );
            CHECK(expect[id].size() , buff.size());

            if(expect[id] == buff )
                expect[id].clear();
        }
        for(std::map<int, std::vector<uint32_t> >::iterator i =  expect.begin() ; i != expect.end() ; i++ )
        {
            CHECK( true , i->second.empty() );
        }
    }

}


TEST(BQ2UKRWriter_Binary)
{

    std::map<int, std::vector<uint32_t> > expect;
    {
        BC2UKWriter_Binary writer(TEST_FILE);
        for( int i = 0 ; i < 100 ; i++)
        {
            std::set<int> unique;
            int num = random() % 10000;
            if( num < 1)
                continue;
            for( int j = 0 ; j < num ; j++ )
            {
                int key = random();
                if(unique.find(key) != unique.end())
                    continue;
                unique.insert(key);
                expect[i].push_back(key);

                //writer.Append(i,key);
            }
            writer.write1barcode(i,expect[i]);
        }
    }
    {
        BC2UKReader_Binary read(TEST_FILE);
        while(true)
        {
            std::vector<uint32_t> buff;
            int id = read.get(buff);
            if( id < 0 )
                break;
            CHECK(true , (buff == expect[id]) );
            CHECK(expect[id].size() , buff.size());

            if(expect[id] == buff )
                expect[id].clear();
        }
        for(std::map<int, std::vector<uint32_t> >::iterator i =  expect.begin() ; i != expect.end() ; i++ )
        {
            CHECK( true , i->second.empty() );
        }
    }

}
