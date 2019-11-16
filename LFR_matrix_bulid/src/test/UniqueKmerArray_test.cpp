#include "UniqueKmerArray.h"
#include <cassert>
#include "Check.h"
#include <iostream>
#include "Test.h"

TEST_MODULE_INIT(UniqueKmerArray)

TEST(UniqueKmerArray_Correct)
{
    const int imax = 100;
    const int jmax = 77;
    UniqueKmerArray* a = new UniqueKmerArray(imax,jmax);
    int array[imax][jmax];
    for(int i = 0 ; i < imax;i++)
    {
        for(int j = 0 ; j < jmax ; j++)
        {
            int rand = random()*j &((0x1<<25)-1);
            array[i][j] = rand;
            a->Append(i,rand);
        }
    }

    for(int i = 0 ; i < imax;i++)
    {
        for(int j = 0 ; j < jmax ; j++)
        {
            CHECK_AND_ONERR(array[i][j] , a->GetBarcodeId(i,j),
                    std::cerr<<"i "<<i<<"j "<<j<<std::endl;);
        }
    }
    delete a;
}

TEST(UniqueKmerArray25Bit_Speed)
{
    const int imax = 0x1<<21;
    const int jmax = 50;

    UniqueKmerArray25Bit * a = new UniqueKmerArray25Bit(imax,jmax);
    {
        for(int i = 0 ; i < imax;i++)
        {
            for(int j = 0 ; j < jmax ; j++)
            {

                a->Append(i,UniqueKmerArray25Bit::BarcodeMask);
            }
        }
    }
    {
        int ja;
        for(int i = 0 ; i < 34; i++)
        {
            for(int i = 0 ; i < imax;i++)
            {
                for(int j = 0 ; j < jmax ; j++)
                {
                    ja = a->GetBarcodeId(i,j);
                }
            }
        }
        std::cout<<ja<<std::endl;
    }
    delete a;
}
TEST( UniqueKmerArray_Speed)
{
    const int imax = 0x1<<21;
    const int jmax = 50;

    UniqueKmerArray* a = new UniqueKmerArray(imax,jmax);
    {
        for(int i = 0 ; i < imax;i++)
        {
            for(int j = 0 ; j < jmax ; j++)
            {

                a->Append(i,UniqueKmerArray25Bit::BarcodeMask);
            }
        }
    }
    {
        int ja=0;
        for(int i = 0 ; i < 34; i++)
        {
            for(int i = 0 ; i < imax;i++)
            {
                for(int j = 0 ; j < jmax ; j++)
                {
                    ja += a->GetBarcodeId(i,j);
                }
            }
        }
        std::cout<<ja<<std::endl;
    }
    delete a;
}
TEST(UniqueKmerArray25Bit_Correct)
{
    // 2^25 cover 30M
    CHECK( (UniqueKmerArray25Bit::BarcodeMask > 1024 * 1024 * 30) , true);
    // append and get vaule from 25 bit arrray .
    {
        const int imax = 100;
        const int jmax = 77;
        UniqueKmerArray25Bit * a = new UniqueKmerArray25Bit(imax,jmax);
        int array[imax][jmax];
        for(int i = 0 ; i < imax;i++)
        {
            for(int j = 0 ; j < jmax ; j++)
            {
                int rand = random()*j &((0x1<<25)-1);
                array[i][j] = rand;
                a->Append(i,rand);
            }
        }

        for(int i = 0 ; i < imax;i++)
        {
            for(int j = 0 ; j < jmax ; j++)
            {
                CHECK_AND_ONERR(array[i][j] , a->GetBarcodeId(i,j),
                        std::cerr<<"i "<<i<<"j "<<j<<std::endl;);
            }
        }
        delete a;
    }
}
