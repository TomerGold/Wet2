#include "DynamicHashTable.h"

DynamicHashTable::DynamicHashTable(int array_size) : array_size(array_size), servers_counter(0) {
    main_servers = new LinkedList<Server>[array_size];
}

DynamicHashTable::~DynamicHashTable() {
    delete[] main_servers;
}

int DynamicHashTable::GetArraySize() const {
    return array_size;
}

void DynamicHashTable::SetArraySize(int new_size) {
    array_size = new_size;
}

int DynamicHashTable::GetServersNumber() const {
    return servers_counter;
}

void DynamicHashTable::SetServersNumber(int new_servers_number) {
    servers_counter = new_servers_number;
}

LinkedList<Server> *DynamicHashTable::GetServersHashTable() const {
    return main_servers;
}

int DynamicHashTable::HashFunction(int server_id) {
    return (server_id % array_size);
}

LinkedList<Server>::node *DynamicHashTable::GetServerById(int server_id) {
    int index = HashFunction(server_id);
    LinkedList<Server>::node *iterator = main_servers[index].start;
    if (iterator == nullptr) {
        return nullptr;
    }
    while (iterator != nullptr) {
        if (server_id == iterator->data.GetServerId()) {
            return iterator;
        }
        iterator = iterator->next;
    }
    return nullptr;
}

bool DynamicHashTable::Find(int server_id) {
    int index = HashFunction(server_id);
    LinkedList<Server>::node *iterator = main_servers[index].start;
    if (iterator == nullptr) {
        return false;
    }
    while (iterator != nullptr) {
        if (server_id == iterator->data.GetServerId()) {
            return true;
        }
        iterator = iterator->next;
    }
    return false;
}

void DynamicHashTable::Insert(Server &server) {
    int index = HashFunction(server.GetServerId());
    main_servers[index].InsertLast(server);// TODO check if works copy c'tor
}

void DynamicHashTable::Remove(int server_id) {
    int index = HashFunction(server_id);
    LinkedList<Server>::node *iterator = main_servers[index].start;
    while (iterator != nullptr) {
        if (server_id == iterator->data.GetServerId()) {
            main_servers[index].RemoveNode(iterator);
            delete iterator;
            break;
        }
        iterator = iterator->next;
    }
}

void DynamicHashTable::ReSizeHashTable(int indicator) {
    if (indicator > 0) {// need to double size;
        int old_size = array_size;
        array_size = array_size * DOUBLE;
        auto *new_arr = new LinkedList<Server>[array_size];
        LinkedList<Server> *old_arr = main_servers;
        main_servers = new_arr;
        for (int i = 0; i < old_size; i++) {
            LinkedList<Server>::node *iterator = old_arr[i].start;
            while (iterator != nullptr) {
                Insert(iterator->data);
                iterator = iterator->next;
            }
        }
        delete[] old_arr;

    } else if (indicator < 0) {
        int old_size = array_size;
        array_size = (array_size / 2) + 1;
        auto *new_arr = new LinkedList<Server>[array_size]; //TODO im not sure it works!!
        LinkedList<Server> *old_arr = main_servers;
        main_servers = new_arr;
        for (int i = 0; i < old_size; i++) {
            LinkedList<Server>::node *iterator = old_arr[i].start;
            while (iterator != nullptr) {
                Insert(iterator->data);
                iterator = iterator->next;
            }
        }
        delete[] old_arr;
    }
}



