#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <stdio.h>
#include <math.h>

std::map<int , float> cppData;
std::map<int,float> perlData;
std::vector<int> perlHead;


void LoadCppData()
{
    std::ifstream in;
    in.open("fromcpp",std::ios::in);
    std::string line;
    getline(in,line);
    for(size_t i = 0 ; i < line.size() ; i++)
        if(line[i] ==  ':')
            line[i] = ' ';
    std::istringstream ism(line);
    int barcode ;
    int size;
    ism>>barcode;
    ism>>size;
    std::cout<<"Load cpp data barcode : "<<barcode <<" total "<<size<<std::endl;
    int index = 0;
    while(!ism.eof())
    {
        int id ; float factor;
        ism>>id>>factor;
        cppData[id] = factor;
        index ++;
    }

}

void LoadPerlData()
{

    std::ifstream in;
    in.open("fromperl",std::ios::in);
    std::string l1;
    std::string l2;
    getline(in,l1);
    getline(in,l2);

    int index = l1.find_first_of('\t');
    std::string l11 = l1.substr(index);
    std::istringstream line1(l11);
    std::istringstream line2(l2);
    
    while(!line1.eof())
    {
        int head;
        line1>>head;
        perlHead.push_back(head);
    }

    int barcode ;
    line2>>barcode;
    std::cout<<"locd prel data barcode "<<barcode<<" size "<<perlHead.size()<<std::endl;
    index = 0 ;
    while(!line2.eof())
    {
        float x;
        line2>>x;
        perlData[perlHead[index]] = x;
        index ++;
    }
    if( (size_t(index) -1) != perlHead.size() )
    {
        std::cout<<"LoadPerlData error"<<std::endl;
    }

}


int main()
{
    LoadCppData();
    LoadPerlData();

    int bg = 0 ;
    int sm = 0 ;
    for( auto i : cppData)
    {
        float diff = (perlData[i.first] - i.second) / i.second;
        if( diff <= 0.05 && diff >= -0.05)
        {
            printf("%d --- %5f <--> %5f\n",i.first,i.second, perlData[i.first]);
            sm ++;
        }
        else
        {
            bg++;
            printf("%d --- %5f <--> %5f  <==\n",i.first,i.second, perlData[i.first]);

        }

        //std::cout<<i.first<<" -- "<<i.second<<" <-> "<<perlData[i.first]<<std::endl;
    }
    std::cout<<" close "<<sm << " diff "<<bg<< " in "<< bg/(bg+sm)<<std::endl;
}
