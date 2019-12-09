#ifndef SEEDCONTROL_H
#define SEEDCONTROL_H
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <sys/time.h>
#include <unordered_map>
#include <pthread.h>
#include <stdlib.h>
#include <mutex>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>
#include "global.h"

/**********************************
 SeedControl class
 Use to control the choice of seed.
 Usage:
    1. use LoadBarcodeInfo to build seed_control_map and barcode_info_map
    2. Before cluster, use get_Empty to make sure the seed_control_map is not empty
    3. Before cluster, use get_seed to get seed barcode
    4. After cluster, use mask_seed to prevent captured seed being seed barcode again
    5. After cluster, use set_Empty to close the seed_control_map
    6. LoadSeedList is used when your provide a seed list rather than the program generate one
 *********************************/

using namespace std;


class SeedControl
{
public:
    SeedControl();
    void init(double unique_rate, int unique_num);
    int get_seed();
    void mask_seed(int barcode);
    void close();
    bool get_Empty();
    unordered_map<int, int>* get_seed_control_map_p();
    void LoadBarcodeInfo(ifstream& in_BarInfo);
    void LoadSeedList();
    void set_Empty();

private:
    double unique_rate_cutoff;
    int unique_num_cutoff;
    unordered_map<int, int> seed_control_map;
    bool Empty;
};


#endif // SEEDCONTROL_H
