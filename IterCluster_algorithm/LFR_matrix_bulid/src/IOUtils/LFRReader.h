#ifndef __LFR_IOUTILS_LFRREADER_H__
#define __LFR_IOUTILS_LFRREADER_H__

#include "SeqReader.h"
#include "Define.h"
struct LFRReader
{
    int prev_id;
    bool eof;

    SeqReader reader;

    LFRReader(const std::string & f ) : prev_id(-1) , eof(false) ,reader(f){}

    bool get(SeqRecord & record)
    {
        bool ret = reader.get(record);
        prev_id = record.barcode;
        eof = !ret;
        return ret;
    }
};

struct LFRPEReader
{

    LFRPEReader(const std::string & f1 , 
            const std::string & f2)
        : first(f1)
        , second(f2)
        , curr_id(-1)
        {}

    int CurrBarcode()const {return curr_id;}

    bool get(SeqRecord & record);

    private:
        LFRReader first;
        LFRReader second;
        int curr_id;
        SeqRecord s1;
        SeqRecord s2;
};

#endif //__LFR_IOUTILS_LFRREADER_H__
