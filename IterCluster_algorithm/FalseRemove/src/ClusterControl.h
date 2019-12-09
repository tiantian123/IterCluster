#ifndef CLUSTERCONTROL_H
#define CLUSTERCONTROL_H
#include <vector>
#include <unordered_map>

using namespace std;
typedef int Seed;
typedef int Barcode;
class ClusterControl
{
    public:
        ClusterControl(unordered_map<Seed, vector<Barcode>>& ClusterSet);
        Seed get_cluster();
        void maskCluster(Seed seed);
        bool isEmpty();
        void set_Empty();
        bool get_Empty();
        void close();
    private:
        bool Empty;
        unordered_map<Seed, vector<Barcode>>* cluster_set_p;
        unordered_map<Seed, int> cluster_control_map;
};

#endif // CLUSTERCONTROL_H
