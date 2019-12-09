#ifndef BARCODE2READ_H_INCLUDED
#define BARCODE2READ_H_INCLUDED

#include "SeqReader.h"
#include <unordered_map>
#include "ReadsInBinary.h"
typedef int Barcode;
using namespace std;
class Barcode2Read
{
    public:
        Barcode2Read(){};
        void add(SeqRecord& sr1, SeqRecord& sr2);
        unordered_map<Barcode,vector<PairInBinary> >* get_hash();

    private:
        unordered_map<Barcode,vector<PairInBinary> > hash_index;

};

#endif // BARCODE2READ_H
