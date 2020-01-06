#include "DataCenter.h"
#include "Server.h"


int DataCenter::GetId() const {
    return id;
}

void DataCenter::SetId(int new_id) {
    id = new_id;
}

AVLTree<Server>& DataCenter::GetServersTree() {
    return serversTree;
}










