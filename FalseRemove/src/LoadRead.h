#ifndef LOADREAD_H_INCLUDED
#define LOADREAD_H_INCLUDED

#include "Barcode2Read.h"

using namespace std;

typedef int Seed;
typedef int Barcode;
void LoadRead2Binary(string filename1,string filename2,Barcode2Read& bar2read);
unordered_map<Seed,vector<Barcode> > LoadClusterSet(string filename);
#endif // LOADREAD_H_INCLUDED
