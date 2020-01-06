#ifndef WET2_DATACENTER_H
#define WET2_DATACENTER_H

#include "LinkedList.h"
#include "Server.h"
#include "AVLTree.h"

class DataCenter {
    int id;
    AVLTree<Server> serversTree; //TODO will the constructor build a tree??

public:
    DataCenter() = default; //After building a data_center we will set its id.

    ~DataCenter() = default; //will call d'tor of AVLTree

    DataCenter(DataCenter &other) = delete;

    DataCenter &operator=(const DataCenter &other) = delete;

    int GetId() const;

    void SetId(int new_id);

    AVLTree<Server>& GetServersTree();
};

#endif //WET2_DATACENTER_H
