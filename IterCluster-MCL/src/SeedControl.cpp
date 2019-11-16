#include "SeedControl.h"
#include "Timer.h"
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
void SplitString(const string& s, vector<double>& v, const string& c);
using namespace std;
SeedControl::SeedControl()
{
    unique_rate_cutoff = 0.0;
    unique_num_cutoff = 0;
    Empty = false;

}

void SeedControl::init(double unique_rate, int unique_num)
{
    this->unique_rate_cutoff = unique_rate;
    this->unique_num_cutoff = unique_num;
}

void SeedControl::mask_seed(int barcode)
{
    if(this->seed_control_map.count(barcode) > 0)
    {
        unordered_map<int,int>::iterator iter = this->seed_control_map.find(barcode);
        iter->second = 0;
    }
}

void SeedControl::close()
{
    this->Empty = true;
}

bool SeedControl::get_Empty()
{
    return this->Empty;
}

void SeedControl::set_Empty()
{
    unordered_map<int, int>::iterator iter = this->seed_control_map.begin();
    for(; iter != this->seed_control_map.end();)
    {
        if(iter->second == 1)
        {
            break;
        }else
        {
            iter++;
        }
    }
    if(iter == this->seed_control_map.end())
    {
        this->close();
    }
}

unordered_map<int, int>* SeedControl::get_seed_control_map_p()
{
    return &(this->seed_control_map);
}

int SeedControl::get_seed()
{
    int barcode = 0;
    for(unordered_map<int, int>::iterator iter = seed_control_map.begin(); iter != seed_control_map.end(); iter++)
    {
        if(iter->second == 1 && iter->first != 0)
        {
            barcode = iter->first;
            break;
        }
    }
    return barcode;
}

//Load the barcode information with absolute ID
void SeedControl::LoadBarcodeInfo(ifstream& in_BarInfo)
{
    Timer t("LoadBarcodeInfo");
    string s;
    while(getline(in_BarInfo, s))
    {
        istringstream strm(s);
        vector<double> temp;
        SplitString(s, temp, "\t");
        double barcode = temp[0];
        double Unikmer_num = temp[1];
        double Kmer_num = temp[2];
        double Unikmer_rate;
        if(Unikmer_num != 0 && Kmer_num != 0)
        {
                Unikmer_rate = Unikmer_num / Kmer_num;
        }else{
                Unikmer_rate = 0.0;
        }
        BarcodeInfo barInfo;
        barInfo.unikmerNum = Unikmer_num;
        barInfo.kmerNum = Kmer_num;
        barInfo.unikmerRate = Unikmer_rate;
        barcode_info_map.insert(make_pair((int)barcode, barInfo));
        if(Unikmer_rate > this->unique_rate_cutoff && Unikmer_num > (double)(this->unique_num_cutoff))
        {
            seed_control_map.insert(make_pair((int)barcode, 1));
        }

    }
    /*if(debug)
    {
        ofstream outfile("seed_control_map.debug");
        for(unordered_map<int, int>::iterator iter = seed_control_map.begin(); iter != seed_control_map.end(); iter++)
        {
            outfile << iter->first << "\t" << iter->second << endl;
        }
        outfile.close();
    }*/
    in_BarInfo.close();

}

//load seed list which user provide
void SeedControl::LoadSeedList()
{
    Timer t("LoadSeedList");
    ifstream infile("Seed.list");

    string s;
    while(getline(infile, s))
    {
        int barcode = (int)atol(s.c_str());
        seed_control_map.insert(make_pair(barcode, 1));
    }

}
