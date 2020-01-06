#ifndef WET2_DATACENTERMANAGER_H
#define WET2_DATACENTERMANAGER_H

#define INCREASE 1
#define DECREASE -1

#include "DataCenter.h"
#include "AVLTree.h"
#include "library2.h"
#include "UF.h"
#include "DynamicHashTable.h"

class DataCenterManager {
    UF DataCentersUF;
    DynamicHashTable ServersHashTable;
    AVLTree<Server> ServersTree;
    int num_of_data_centers;

    //merges two sorted array into new array
    static void Merge(Server *arr1, Server *arr2, int size1, int size2, Server *new_arr);

    static int CalcHeight(int num);

    static int CalcNodesNum(int count);

public:
    explicit DataCenterManager(int num);

    ~DataCenterManager() = default;

    DataCenterManager(DataCenterManager &dataCenterManager) = delete;

    DataCenterManager &operator=(DataCenterManager &dataCenterManager) = delete;

    StatusType MergeDataCenters(int dataCenter1, int dataCenter2);

    StatusType AddServer(int dataCenterID, int serverID);

    StatusType RemoveServer(int serverID);

    StatusType SetTraffic(int serverID, int traffic);

    StatusType SumHighestTrafficServers(int dataCenterID, int k, int *traffic);

    class ErrorCodes : public std::exception {
    };

    class ALLOCATION_ERROR : public ErrorCodes {
    };


    void SumHighK(int k, AVLTree<Server> tree);
};


#endif //WET2_DATACENTERMANAGER_H
