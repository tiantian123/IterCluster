//#define BEBUG
#include "Barcode2Read.h"
#include "SeqReader.h"
#include "SeqOperator.h"
#include "ReadsInBinary.h"
#include "LoadRead.h"
#include <fstream>
#include <assert.h>
#include "Timer.h"


using namespace std;
void LoadRead2Binary(string filename1,string filename2,Barcode2Read& bar2read)
{
    Timer t("LoadRead2Binary");
    SeqReader fq1(filename1,1);
    SeqReader fq2(filename2,1);
    SeqRecord pair1;
    SeqRecord pair2;
    while(fq1.get(pair1) && fq2.get(pair2))
    {
        bar2read.add(pair1, pair2);
    }
    #ifdef BEBUG
    ofstream debug("debug_hash");
    assert(debug.is_open());
    for(unordered_map<Barcode,vector<PairInBinary> >::iterator iter = bar2read.get_hash()->begin(); iter != bar2read.get_hash()->end(); iter++)
    {
        Barcode barcode = iter->first;
        for(vector<PairInBinary>::iterator iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
        {
            debug << barcode << ":\t" << (*iter2).read_P1.read_1 << "\t" << (*iter2).read_P1.read_2 << "\t" << (*iter2).read_P1.read_3 << "\t" << (*iter2).read_P1.read_4 << "\t" << (*iter2).read_P2.read_1 << "\t" << (*iter2).read_P2.read_2 << "\t" << (*iter2).read_P2.read_3 << "\t" << (*iter2).read_P2.read_4 << "\n";
        }
    }
    debug.close();
    #endif // BEBUG
}
unordered_map<Seed,vector<Barcode> > LoadClusterSet(string filename)
{
    Timer t("LoadClusterSet");
    ifstream in(filename);
    assert(in.is_open());
    unordered_map<Seed,vector<Barcode> > seed2barcode;
    string s;
    while(getline(in,s)){
        vector<Barcode> tar_barcode_array;
        istringstream strm(s);
        string se;
        getline(strm,se,'\t');
        Seed seed = atoi(se.c_str());
        string tar_barcode;
        while(getline(strm,tar_barcode,'\t'))
        {
            int Tar_barcode = atoi(tar_barcode.c_str());
            tar_barcode_array.push_back(Tar_barcode);
        }
        seed2barcode.insert(pair<Seed,vector<Barcode> >(seed, tar_barcode_array));
    }
    return seed2barcode;
}



