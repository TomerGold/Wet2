#ifndef WET2_LINKEDLIST_H
#define WET2_LINKEDLIST_H

template<typename T>
class LinkedList {
    struct node {
        T data;
        node *next;
        node *prev;

        node(T new_data, node *new_next, node *new_prev) : data(new_data), next(new_next), prev(new_prev) {}
    };

    node *start;
    node *end;

    friend class Server; //Server using pointer to Node
    friend class DataCenter; // DataCenter using and building Nodes
    friend class DynamicHashTable; //DynamicHashTable will use List nodes for iterator
    friend class DataCenterManager; //for iterating over a list

public:
    LinkedList() : start(nullptr), end(nullptr) {};

    explicit LinkedList(int num_of_items) : start(nullptr), end(nullptr) {
        for (int i = 0; i < num_of_items; ++i) {
            InsertLast(i);
        }
    }

    ~LinkedList() {
        while (start != nullptr) {
            node *temp = start;
            start = start->next;
            delete temp;
        }
    }

    LinkedList(const LinkedList &otherList) {
        start = nullptr;
        end = nullptr;
        if (otherList.start == nullptr) {
            return;
        }
        InsertLast(otherList.start->data);
        node *temp = otherList.start->next;
        while (temp != nullptr) {
            InsertLast(temp->data);
            temp = temp->next;
        }
    }

    LinkedList &operator=(const LinkedList &otherList) {
        if (otherList.start == nullptr) {
            start = nullptr;
            end = nullptr;
            return *this;
        }
        node *iterator, *next_node;
        iterator = start;
        if (iterator != nullptr) {
            next_node = iterator->next;
            while (next_node != nullptr) {
                delete iterator;
                iterator = next_node;
                next_node = iterator->next;
            }
            start = nullptr;
            end = nullptr;
        }
        InsertLast(otherList.start->data);
        node *temp = otherList.start->next;
        while (temp != nullptr) {
            this->InsertLast(temp->data);
            temp = temp->next;
        }
        return *this;
    }

    bool IsEmpty() const {
        return (!start);
    }

//creates and insert (in the end of the list) a node using the data provided
    void InsertLast(T data) {
        end = new node(data, nullptr, end);
        if (end->prev != nullptr) {
            end->prev->next = end;
        }
        if (IsEmpty()) {
            start = end;
        }
    }

//removes a node from the list
    void RemoveNode(node *ptr) {
        if (ptr->prev == nullptr) {
            RemoveFirst(ptr);
        } else if (ptr->next == nullptr) {
            RemoveLast(ptr);
        } else RemoveMiddle(ptr);
    }

//removes a Node if it is the first node in the list (used by RemoveNode function)
    void RemoveFirst(node *ptr) {
        start = start->next;
        if (start != nullptr) {
            start->prev = nullptr;
        } else end = nullptr;
        ptr->next = nullptr;
    }

//removes a Node if it is the last node in the list (used by RemoveNode function)
    void RemoveLast(node *ptr) {
        end = end->prev;
        if (end != nullptr) {
            end->next = nullptr;
        } else start = nullptr;
        ptr->prev = nullptr;
    }

//removes a Node if it is in the center of the list - not first or last (used by RemoveNode function)
    void RemoveMiddle(node *ptr) {
        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;
        ptr->next = nullptr;
        ptr->prev = nullptr;
    }
};


#endif //WET2_SERVER_H
