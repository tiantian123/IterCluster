#include "SeqReader.h"
#include "Test.h"

TEST_MODULE_INIT(SeqReader)

#ifndef TEST_DATA_PATH
#define TEST_DATA_PATH "../test/Data/"
#endif



TEST(SeqReader_Correct_fqfile)
{
    SeqReader reader(TEST_DATA_PATH"/testRead1.fq");
    SeqRecord record;

    reader.get(record);
    CHECK(1,record.barcode);
    CHECK("AGCGTGTGGGGCCGCAGGCAGGGTGGTGGCAGCTCGCGGCTCTGAACTTTCCCAAAGGTCATTAGGGATGTGATGGGAATAAGGGATAGATTATTTACTA",record.seq);

    reader.get(record);
    CHECK(1,record.barcode);
    CHECK("GTGTCCTTTGGAAAGACAGAGTGGTGCAGCCAGTGTAGCTCTGAAGGTCTACAATGAAGCCATGTTTCATCTTTCAGACAGCTGTGGTACCTGGACATCA",record.seq);
}


TEST(SeqReader_Correct_fqgzfile)
{
    SeqReader reader(TEST_DATA_PATH"/subRead1.fq.gz");
    SeqRecord record;

    reader.get(record);
    CHECK(1,record.barcode);
    CHECK("GGCGTGTGGGGCCGCAGGCAGGGTGGTGGCAGCTCGCGGCTCTGAACTTTCCCAAAGGTCATTAGGGATGTGATGGGAATAAGGGATAGATTATTTACTA",record.seq);

    reader.get(record);
    CHECK(1,record.barcode);
    CHECK("GTGTCCTTTGGAAAGACAGAGTGGTGCAGCCAGTGTAGCTCTGAAGGTCTACAATGAAGCCATGTTTCATCTTTCAGACAGCTGTGGTACCTGGACATCA",record.seq);
    for( int i = 0 ; i < 999 ; i++ )
        reader.get(record);

    CHECK(6,record.barcode);
    CHECK("ATACGTATCCTACATGTATCCTATATATACGTATCCTATATATACGCTGTCTCTTATACACATCTTAGGAAGACAAGCACTGACGACATGATCACCAAGG" ,record.seq);
}



