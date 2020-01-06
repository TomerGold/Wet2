
#include "library2.h"
#include "DataCenterManager.h"

void *Init(int n) {
    auto *DS = new DataCenterManager(n);
    return (void *) DS;
}


StatusType MergeDataCenters(void *DS, int dataCenter1, int dataCenter2){
    if (!DS) {
        return INVALID_INPUT;
    }
    return ((DataCenterManager *) DS)-> MergeDataCenters(dataCenter1, dataCenter2);
}


StatusType AddServer(void *DS, int dataCenterID, int serverID) {
    if (!DS) {
        return INVALID_INPUT;
    }
    return ((DataCenterManager *) DS)->AddServer(dataCenterID, serverID);
}

StatusType RemoveServer(void *DS, int serverID) {
    if (!DS) {
        return INVALID_INPUT;
    }
    return ((DataCenterManager *) DS)->RemoveServer(serverID);
}

StatusType SetTraffic(void *DS, int serverID, int traffic) {
    if (!DS) {
        return INVALID_INPUT;
    }
    return ((DataCenterManager *) DS)->SetTraffic(serverID, traffic);
}

StatusType SumHighestTrafficServers(void *DS, int dataCenterID, int k, int *traffic) {
    if (!DS) {
        return INVALID_INPUT;
    }
    return ((DataCenterManager *) DS)->SumHighestTrafficServers(dataCenterID, k, traffic);
}

void Quit(void **DS) {
    if (*DS != nullptr) {
        delete ((DataCenterManager *) *DS);
        *DS = nullptr;
    }
}