#ifndef READSINBINARY_H_INCLUDED
#define READSINBINARY_H_INCLUDED
#include <stdint.h>

using namespace std;


struct ReadsInBinary
{
    //data in four path, 32+32+32+8 to represent a 100 length reads
    uint64_t read_1;
    uint64_t read_2;
    uint64_t read_3;
    uint8_t read_4;
    ReadsInBinary():read_1(0),read_2(0),read_3(0),read_4(0){}
    ReadsInBinary(const ReadsInBinary& s):read_1(s.read_1),read_2(s.read_2),read_3(s.read_3),read_4(s.read_4){}
};

struct PairInBinary
{
    ReadsInBinary read_P1;
    ReadsInBinary read_P2;
    PairInBinary()
    {
        ReadsInBinary read_P1;
        ReadsInBinary read_P2;
    }
    PairInBinary(const PairInBinary& s)
    {
        read_P1.read_1 = s.read_P1.read_1;
        read_P1.read_2 = s.read_P1.read_2;
        read_P1.read_3 = s.read_P1.read_3;
        read_P1.read_4 = s.read_P1.read_4;
        read_P2.read_1 = s.read_P2.read_1;
        read_P2.read_2 = s.read_P2.read_2;
        read_P2.read_3 = s.read_P2.read_3;
        read_P2.read_4 = s.read_P2.read_4;
    }
};



#endif // READSINBINARY_H_INCLUDED
