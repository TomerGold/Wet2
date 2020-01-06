#ifndef WET2_DYNAMICHASHTABLE_H
#define WET2_DYNAMICHASHTABLE_H

#include "LinkedList.h"
#include "Server.h"

#define DOUBLE 2

class DynamicHashTable {
    LinkedList<Server> *main_servers;
    int array_size;
    int servers_counter;

public:
    explicit DynamicHashTable(int array_size);

    ~DynamicHashTable();

    DynamicHashTable(const DynamicHashTable &other) = delete;

    DynamicHashTable &operator=(const DynamicHashTable &other) = delete;

    int GetArraySize() const;

    void SetArraySize(int new_size);

    int GetServersNumber() const;

    void SetServersNumber(int new_servers_number);

    LinkedList<Server> *GetServersHashTable() const;

    int HashFunction(int server_id);

    LinkedList<Server>::node* GetServerById(int server_id);

    bool Find(int server_id);

    void Insert(Server& server);

    void Remove(int server_id);

    void ReSizeHashTable(int indicator);

};


#endif //WET2_DYNAMICHASHTABLE_H
