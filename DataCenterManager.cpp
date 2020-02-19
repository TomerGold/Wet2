#include "DataCenterManager.h"

DataCenterManager::DataCenterManager(int num) : DataCentersUF(num), ServersHashTable(num),
                                                ServersTree(), num_of_data_centers(num) {}


StatusType DataCenterManager::MergeDataCenters(int dataCenter1, int dataCenter2) {
    if (dataCenter1 > num_of_data_centers || dataCenter1 <= 0 || dataCenter2 > num_of_data_centers || dataCenter2 <= 0) {
        return INVALID_INPUT;
    }
    int num_of_servers1 = 0, num_of_servers2 = 0, index1 = 0, index2 = 0, new_data_center_index = 0, sum_of_servers = 0;
    index1 = DataCentersUF.Find(dataCenter1);
    if (DataCentersUF.GetDataCenters()[index1].GetServersTree().GetRoot() != nullptr) {
        num_of_servers1 = DataCentersUF.GetDataCenters()[index1].GetServersTree().GetRoot()->GetNumOfServers() + 1;
    }
    index2 = DataCentersUF.Find(dataCenter2);
    if (DataCentersUF.GetDataCenters()[index2].GetServersTree().GetRoot() != nullptr) {
        num_of_servers2 = DataCentersUF.GetDataCenters()[index2].GetServersTree().GetRoot()->GetNumOfServers() + 1;
    }
    sum_of_servers = num_of_servers1 + num_of_servers2;
    if (sum_of_servers == 0 || (index1 == index2)) { //both DataCenters dont have servers no need to merge trees
        DataCentersUF.Union(dataCenter1, dataCenter2);
        return SUCCESS;
    }
    try {
        auto *temp_array_dc1 = new Server[num_of_servers1];
        auto *temp_array_dc2 = new Server[num_of_servers2];
        auto *temp_array_both = new Server[sum_of_servers];
        DataCentersUF.GetDataCenters()[index1].GetServersTree().InorderTreeFillArray(temp_array_dc1); //fill array from tree 1
        DataCentersUF.GetDataCenters()[index2].GetServersTree().InorderTreeFillArray(temp_array_dc2); //fill array from tree 2
        Merge(temp_array_dc1, temp_array_dc2, num_of_servers1, num_of_servers2, temp_array_both); //merge the arrays together

        AVLNode<Server> *old_root1 = DataCentersUF.GetDataCenters()[index1].GetServersTree().GetRoot();//Delete the two old trees
        AVLNode<Server> *old_root2 = DataCentersUF.GetDataCenters()[index2].GetServersTree().GetRoot();
        DataCentersUF.GetDataCenters()[index1].GetServersTree().DestroyTree(old_root1);
        DataCentersUF.GetDataCenters()[index2].GetServersTree().DestroyTree(old_root2);
        DataCentersUF.GetDataCenters()[index1].GetServersTree().SetRoot(nullptr);
        DataCentersUF.GetDataCenters()[index2].GetServersTree().SetRoot(nullptr);

        new_data_center_index = DataCentersUF.Union(dataCenter1, dataCenter2);

        int new_height = CalcHeight(sum_of_servers); //height for the new merged tree
        int complete_tree_nodes = CalcNodesNum(new_height); //num of nodes before reduction in new tree
        AVLNode<Server> *new_root = BuildCompleteTree<Server>(new_height, nullptr); //build new tree
        DataCentersUF.GetDataCenters()[new_data_center_index].GetServersTree().SetRoot(new_root);
        DataCentersUF.GetDataCenters()[new_data_center_index].GetServersTree().RemoveLeafsTree(complete_tree_nodes,
                                                                                               sum_of_servers);
        DataCentersUF.GetDataCenters()[new_data_center_index].GetServersTree().InorderTreeFillTree(temp_array_both);
        DataCentersUF.GetDataCenters()[new_data_center_index].GetServersTree().UpdateNewTree();

        delete[] temp_array_dc1;
        delete[] temp_array_dc2;
        delete[] temp_array_both;
    }

    catch (std::bad_alloc &) {
        throw ALLOCATION_ERROR{};
    }
    return SUCCESS;
}

StatusType DataCenterManager::AddServer(int dataCenterID, int serverID) {
    if (dataCenterID <= 0 || dataCenterID > num_of_data_centers || serverID <= 0) {
        return INVALID_INPUT;
    }
    if (ServersHashTable.Find(serverID) == true) {
        return FAILURE;
    }
    try {
        Server new_server(serverID, dataCenterID, 0);
        ServersHashTable.Insert(new_server);
        ServersHashTable.SetServersNumber(ServersHashTable.GetServersNumber() + 1);
    }
    catch (std::bad_alloc &) {
        throw ALLOCATION_ERROR{};
    }
    if (ServersHashTable.GetServersNumber() == ServersHashTable.GetArraySize()) {
        try {
            ServersHashTable.ReSizeHashTable(INCREASE); //the hash table becomes bigger
        }
        catch (std::bad_alloc &) {
            throw ALLOCATION_ERROR{};
        }
    }

    return SUCCESS;
}

StatusType DataCenterManager::RemoveServer(int serverID) {
    if (serverID <= 0) {
        return INVALID_INPUT;
    }
    if (ServersHashTable.Find(serverID) == false) {
        return FAILURE;
    }
    LinkedList<Server>::node *server_to_remove = ServersHashTable.GetServerById(serverID);
    if (server_to_remove->data.GetTraffic() != 0) {
        int index = DataCentersUF.Find(server_to_remove->data.GetDataCenterId());
        DataCentersUF.GetDataCenters()[index].GetServersTree().Remove(server_to_remove->data);
        ServersTree.Remove(server_to_remove->data);
    }
    ServersHashTable.Remove(server_to_remove->data.GetServerId());
    ServersHashTable.SetServersNumber(ServersHashTable.GetServersNumber() - 1);
    if (ServersHashTable.GetServersNumber() <= ServersHashTable.GetArraySize() / 4) {
        try {
            ServersHashTable.ReSizeHashTable(DECREASE); //the hash table becomes smaller
        }
        catch (std::bad_alloc &) {
            throw ALLOCATION_ERROR{};
        }
    }
    return SUCCESS;
}

StatusType DataCenterManager::SetTraffic(int serverID, int traffic) {
    if (serverID <= 0 || traffic < 0) {
        return INVALID_INPUT;
    }
    if (ServersHashTable.Find(serverID) == false) {
        return FAILURE;
    }
    LinkedList<Server>::node *server_to_change = ServersHashTable.GetServerById(serverID);
    if (traffic == server_to_change->data.GetTraffic()) { //nothing to do..
        return SUCCESS;
    } else if (traffic == 0 && server_to_change->data.GetTraffic() != 0) {
        int temp_data_center_id = server_to_change->data.GetDataCenterId();
        try {
            RemoveServer(serverID); // remove from all of the trees and arrays
            AddServer(temp_data_center_id, serverID); //add back but only to hash table because traffic=0
        }
        catch (std::bad_alloc &) {
            throw ALLOCATION_ERROR{};
        }
    } else if (traffic != 0 && server_to_change->data.GetTraffic() == 0) {
        try {
            Server new_server(serverID, server_to_change->data.GetDataCenterId(), traffic);
            ServersTree.Insert(new_server);
            int index = DataCentersUF.Find(new_server.GetDataCenterId());
            DataCentersUF.GetDataCenters()[index].GetServersTree().Insert(new_server);
        }
        catch (std::bad_alloc &) {
            throw ALLOCATION_ERROR{};
        }
        server_to_change->data.SetTraffic(traffic);
    } else { //traffic !=0 and servers traffic !=0
        try {
            Server new_server(serverID, server_to_change->data.GetDataCenterId(), traffic);
            ServersTree.Remove(server_to_change->data);
            ServersTree.Insert(new_server);
            int index = DataCentersUF.Find(new_server.GetDataCenterId());
            DataCentersUF.GetDataCenters()[index].GetServersTree().Remove(server_to_change->data);
            DataCentersUF.GetDataCenters()[index].GetServersTree().Insert(new_server);
        }
        catch (std::bad_alloc &) {
            throw ALLOCATION_ERROR{};
        }
        server_to_change->data.SetTraffic(traffic);
    }
    return SUCCESS;
}

StatusType DataCenterManager::SumHighestTrafficServers(int dataCenterID, int k, int *traffic) {
    if (k < 0 || dataCenterID > num_of_data_centers || dataCenterID < 0 || traffic == nullptr) {
        return INVALID_INPUT;
    }
    if (k == 0) {
        *traffic = 0;
        return SUCCESS;
    }
    if (dataCenterID == 0) {
        if (ServersTree.GetRoot() != nullptr) {
            if (k >= ServersTree.GetRoot()->GetNumOfServers() + 1) {
                *traffic = ServersTree.GetRoot()->GetSumOfTraffic() + ServersTree.GetRoot()->GetData().GetTraffic();
            } else {
                *traffic = ServersTree.GetRoot()->SumHighK(k);
            }
        } else {
            *traffic = 0;
        }
    } else {
        int index = DataCentersUF.Find(dataCenterID);
        if (DataCentersUF.GetDataCenters()[index].GetServersTree().GetRoot() != nullptr) {
            if (k >= DataCentersUF.GetDataCenters()[index].GetServersTree().GetRoot()->GetNumOfServers() + 1) {
                *traffic = DataCentersUF.GetDataCenters()[index].GetServersTree().GetRoot()->GetSumOfTraffic() +
                           DataCentersUF.GetDataCenters()[index].GetServersTree().GetRoot()->GetData().GetTraffic();
            } else {
                *traffic = DataCentersUF.GetDataCenters()[index].GetServersTree().GetRoot()->SumHighK(k);
            }
        } else {
            *traffic = 0;
        }
    }
    return SUCCESS;
}

void DataCenterManager::Merge(Server *arr1, Server *arr2, int size1, int size2, Server *new_arr) {
    {
        int i = 0, j = 0, k = 0;

        while (i < size1 && j < size2) {
            if (arr2[j] > arr1[i]) {
                new_arr[k++] = arr1[i++];
            } else {
                new_arr[k++] = arr2[j++];
            }
        }

        while (i < size1) {
            new_arr[k++] = arr1[i++];
        }
        while (j < size2) {
            new_arr[k++] = arr2[j++];
        }
    }

}

int DataCenterManager::CalcHeight(int num) {
    int count = 0;
    while (num != 1) {
        count++;
        num = num / 2;
    }
    return count;
}

int DataCenterManager::CalcNodesNum(int count) {
    int nodes_num = 1;
    if (count == 0) {
        return nodes_num;
    }
    while (count >= 0) {
        nodes_num *= 2;
        count--;
    }
    return (nodes_num - 1);
}






