#include "LFRReader.h"

bool LFRPEReader::get(SeqRecord &record)
{
    if(first.eof && second.eof)
        return false ;
    if( s1.barcode != -1 && s2.barcode != -1 )
    {
        if(s1.barcode > s2.barcode )
        {
            curr_id = s2.barcode ;
            record = s2 ;
            s2.barcode = -1;
            return true;
        }
        else
        {
            curr_id = s1.barcode;
            record = s1;
            s1.barcode = -1;
            return true;
        }
    }

    if(first.eof && s1.barcode == -1 && s2.barcode != -1)
    {
        curr_id = s2.barcode ;
        record = s2 ;
        s2.barcode = -1;
        return true;
    }

    if(second.eof && s2.barcode == -1 && s1.barcode != -1)
    {
        curr_id = s1.barcode;
        record = s1;
        s1.barcode = -1;
        return true;
    }

    if(first.prev_id == curr_id &&  ! first.eof)
    {
        if(first.get(record))
        {
            if(first.prev_id != curr_id)
            {
                s1 = record;
                return get(record);
            }
            else
                return true;
        }
        else
            return get(record);
    }
    else if( second.prev_id == curr_id && ! second.eof)
    {
        if(second.get(record))
        {
            if(second.prev_id != curr_id)
            {
                s2 = record ;
                return get(record);
            }
            else
                return true;
        }
        else
            return get(record);
    }

    return false;
}
