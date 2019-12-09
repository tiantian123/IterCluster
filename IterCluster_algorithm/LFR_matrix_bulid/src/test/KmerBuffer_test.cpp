#include "Test.h"
#include "Read2KmerBuffer.h"

TEST_MODULE_INIT(KmerBuffer)

TEST(KMerBuffer_chopkmer)
{
    Read2KmerBuffer buffer;
    buffer.Init(2);
    int prev  = KValue;
    KValue = 4;
    // A  G  C T
    // 00 11 01 10
    buffer.seqs[0].seq = "AGCTA";
    buffer.chopRead2Kmer(0);
    CHECK(0x36,buffer.kmers[0][0]);
    CHECK(0x8d,buffer.kmers[0][1]);
    KValue = prev;
}
