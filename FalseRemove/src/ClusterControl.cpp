#include "ClusterControl.h"
#include <iostream>

ClusterControl::ClusterControl(unordered_map<Seed, vector<Barcode>>& ClusterSet)
{
    Empty = false;
    cluster_set_p = &ClusterSet;
    for(unordered_map<Seed, vector<Barcode>>::iterator iter = cluster_set_p->begin(); iter != cluster_set_p->end(); iter++)
    {
        this->cluster_control_map.insert(pair<Seed, int>(iter->first, 1));
    }
}

Seed ClusterControl::get_cluster()
{
    Seed seed = 0;
    for(unordered_map<Seed, int>::iterator iter = cluster_control_map.begin(); iter != cluster_control_map.end(); iter++)
    {
        if(iter->second == 1 && iter->first != 0)
        {
            seed = iter->first;
            break;
        }
    }
    if(seed == 0)
    {
        cerr << "illegal seed in ClusterControl::get_cluster()" << endl;
    }
    return seed;
}

void ClusterControl::maskCluster(Seed seed)
{
    if(this->cluster_control_map.count(seed) > 0)
    {
        unordered_map<Seed,int>::iterator iter = this->cluster_control_map.find(seed);
        iter->second = 0;
    }
}

void ClusterControl::close()
{
    this->Empty = true;
}

bool ClusterControl::get_Empty()
{
    return this->Empty;
}

void ClusterControl::set_Empty()
{
    unordered_map<Seed, int>::iterator iter = this->cluster_control_map.begin();
    for(; iter != this->cluster_control_map.end();)
    {
        if(iter->second == 1)
        {
            break;
        }else
        {
            iter++;
        }
    }
    if(iter == this->cluster_control_map.end())
    {
        this->close();
    }
}
