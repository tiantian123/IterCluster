#include "Test.h"
#include "LFRReader.h"

TEST_MODULE_INIT(LFRReader)

#ifndef TEST_DATA_PATH
#define TEST_DATA_PATH "../test/Data/"
#endif

TEST(LFRReader_Correct)
{
    SeqRecord record;
    LFRReader reader(TEST_DATA_PATH"testRead1.fq");
    CHECK(false,reader.eof);
    CHECK(-1,reader.prev_id);
    reader.get(record);
    CHECK("AGCGTGTGGGGCCGCAGGCAGGGTGGTGGCAGCTCGCGG"
          "CTCTGAACTTTCCCAAAGGTCATTAGGGATGTGATGGGA"
          "ATAAGGGATAGATTATTTACTA",record.seq);
    CHECK(1,reader.prev_id);
    for(int i = 0 ; i < 8; i++)
        reader.get(record);
    CHECK("ATCTTGCTGCCTTCATTCATCGTTATGTTTCTGAGCCTCG"
          "TCCATTTTGTTGAGTCTAAATGTAGTTTATATACTGTCTC"
          "TTATACACATCTTAGGAAGG",record.seq);

    CHECK(3,reader.prev_id);
    CHECK(false,reader.get(record));
    CHECK(true,reader.eof);
}

TEST(LFRPEReader_Correct)
{
    LFRPEReader reader(TEST_DATA_PATH"testRead1.fq",TEST_DATA_PATH"testRead2.fq");
    SeqRecord record;
    CHECK(-1,reader.CurrBarcode());

    reader.get(record);
    CHECK("AGCGTGTGGGGCCGCAGGCAGGGTGGTGGCAGCTCGCGG"
          "CTCTGAACTTTCCCAAAGGTCATTAGGGATGTGATGGGA"
          "ATAAGGGATAGATTATTTACTA",record.seq);
    CHECK(1,reader.CurrBarcode());
    for(int i = 0 ; i < 4; i++)
        reader.get(record);

    CHECK("GTGTCCTTTGGAAAGACAGAGTGGTGCAGCCAGTGTAGCT"
          "CTGAAGGTCTACAATGAAGCCATGTTTCATCTTTCAGACA"
          "GCTGTGGTACCTGGACATCA",record.seq);
    
    CHECK(1,reader.CurrBarcode());
    for(int i = 0 ; i < 4; i++)
        reader.get(record);
    CHECK("ATCTTGCTGCCTTCATTCATCGTTATGTTTCTGAGCCTCG"
          "TCCATTTTGTTGAGTCTAAATGTAGTTTATATACTGTCTC"
          "TTATACACATCTTAGGAAGA",record.seq);

    CHECK(2,reader.CurrBarcode());
    for(int i = 0 ; i <7 ; i++)
        reader.get(record);
    CHECK("GTCGTGTGGGGCCGCAGGCAGGGTGGTGGCAGCTCGCGGCT"
          "CTGAACTTTCCCAAAGGTCATTAGGGATGTGATGGGAATAA"
          "GGGATAGATTATTTACTA",record.seq);
    CHECK(3,record.barcode);
    reader.get(record);
    CHECK(true,reader.get(record));
    reader.get(record);
    CHECK(false,reader.get(record));
}
