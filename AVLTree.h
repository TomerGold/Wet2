//
// Created by tomer on 02/01/2020.
//

#ifndef WET2_AVLTREE_H
#define WET2_AVLTREE_H


#define NEGATIVE_HEIGHT -1

#include <iostream>
#include "library2.h"


typedef enum {
    NO_CHILDREN,
    ONLY_LEFT_CHILD,
    ONLY_RIGHT_CHILD,
    HAS_BOTH_CHILDREN
} ChildrenStatus;


template<typename T>
class AVLNode { //A generic Node in the Tree
    T data;
    int height;
    int num_of_servers;
    int sum_of_traffic;
    AVLNode *left_child;
    AVLNode *right_child;
    AVLNode *parent;

public:
    AVLNode() : data(), left_child(nullptr), right_child(nullptr), parent(nullptr) {
        height = 0;
        num_of_servers = 0;
        sum_of_traffic = 0;
    }

    explicit AVLNode(T &new_data) : data(new_data), left_child(nullptr), right_child(nullptr), parent(nullptr) {
        height = 0;
        num_of_servers = 0;
        sum_of_traffic = 0;
    }

    ~AVLNode() = default; //T has a d'tor

    AVLNode(const AVLNode &other) = delete; //TODO maybe not?

    AVLNode &operator=(const AVLNode &other) = delete; //TODO maybe not?

    T &GetData() {
        return data;
    }

    int GetHeight() const {
        return height;
    }

    void SetHeight() {
        int left = NEGATIVE_HEIGHT, right = NEGATIVE_HEIGHT, max = 0;
        if (left_child != nullptr) {
            left = left_child->height;
        }
        if (right_child != nullptr) {
            right = right_child->height;
        }
        if (left > right) {
            max = left;
        } else max = right;
        height = max + 1;  //includes the node itself
        SetNumOfServers(); //will always need to do this if changing height //TODO is it true to do it here?
        SetSumOfTraffic(); //will always need to do this if changing height //TODO is it true to do it here?
    }

    void SetHeightWithNum(int new_hight) {
        height = new_hight;
    }

    int GetNumOfServers() const {
        return num_of_servers;
    }

    void SetNumOfServers() {
        int left = 0, right = 0;
        if (left_child != nullptr) {
            left = left_child->num_of_servers + 1;
        }
        if (right_child != nullptr) {
            right = right_child->num_of_servers + 1;
        }
        num_of_servers = left + right; //without the node itself
    }

    int GetSumOfTraffic() const {
        return sum_of_traffic;
    }

    void SetSumOfTraffic() {
        int left = 0, right = 0;
        if (left_child != nullptr) {
            left = left_child->sum_of_traffic + left_child->data.GetTraffic(); //TODO NOT GENERIC
        }
        if (right_child != nullptr) {
            right = right_child->sum_of_traffic + right_child->data.GetTraffic(); //TODO NOT GENERIC
        }
        sum_of_traffic = left + right; //without the node itself
    }

    AVLNode *GetLeftChild() const {
        return left_child;
    }

    void SetLeftChild(AVLNode *child) {
        left_child = child;
    }

    AVLNode *GetRightChild() const {
        return right_child;
    }

    void SetRightChild(AVLNode *child) {
        right_child = child;
    }

    AVLNode *GetParent() {
        return parent;
    }

    void SetParent(AVLNode *new_parent) {
        parent = new_parent;
    }

    int GetBalance() {
        int left = NEGATIVE_HEIGHT, right = NEGATIVE_HEIGHT;
        if (left_child != nullptr) {
            left = left_child->height;
        }
        if (right_child != nullptr) {
            right = right_child->height;
        }
        return (left - right); //according to balance factor calculation
    }

    void InorderReverseNode(T *array, int &counter) { //TODO is needed?
        if (right_child != nullptr) {
            right_child->InorderReverseNode(array, counter);
        }
        array[counter] = data;
        counter++;
        if (left_child != nullptr) {
            left_child->InorderReverseNode(array, counter);
        }
    }

    void InorderNodeFillArray(T *array, int &counter) {
        if (left_child != nullptr) {
            left_child->InorderNodeFillArray(array, counter);
        }
        array[counter] = data;
        counter++;
        if (right_child != nullptr) {
            right_child->InorderNodeFillArray(array, counter);
        }
    }

    void InorderNodeFillTree(T *array, int &counter) {
        if (left_child != nullptr) {
            left_child->InorderNodeFillTree(array, counter);
        }
        data = array[counter];
        counter++;
        if (right_child != nullptr) {
            right_child->InorderNodeFillTree(array, counter);
        }
    }

    AVLNode<T> *DeleteLeafs(int &counter) {
        if (counter <= 0) {
            return this;
        }
        if(right_child == nullptr && left_child == nullptr){
            left_child = nullptr;
            right_child = nullptr;
            parent = nullptr;
            delete this;
            counter--;
            return nullptr;
        }
        right_child = right_child->DeleteLeafs(counter);
        left_child = left_child->DeleteLeafs(counter);
        return this;
    }

    void UpdateNode(AVLNode<T> *node) {
        if (node->left_child != nullptr) {
            node->UpdateNode(node->left_child);
        }
        if (node->right_child != nullptr) {
            node->UpdateNode(node->right_child);
        }
        node->SetHeight();
    }

    int SumHighK(int k) {
        if (k == 0) {
            return 0;
        }
        if (right_child == nullptr) {
            if (left_child == nullptr) {
                return data.GetTraffic();
            } else {
                return data.GetTraffic() + left_child->SumHighK(k - 1);
            }
        } else {
            if (k == right_child->num_of_servers + 1) {
                return right_child->data.GetTraffic() + right_child->sum_of_traffic;
            } else if (k > right_child->num_of_servers + 1) {
                if (left_child != nullptr) {
                    return (right_child->data.GetTraffic() + right_child->sum_of_traffic + data.GetTraffic() +
                            left_child->SumHighK(k - (right_child->num_of_servers + 1) - 1));
                } else {
                    return right_child->data.GetTraffic() + right_child->sum_of_traffic + data.GetTraffic();
                }
            }
            return right_child->SumHighK(k);
        }
    }
};

template<typename T>
AVLNode<T> *BuildCompleteTree(int new_height, AVLNode<T> *node_parent) {
    if (new_height < 0) {
        return nullptr;
    }
    auto *node = new AVLNode<T>();
    node->SetParent(node_parent);
    node->SetLeftChild(BuildCompleteTree<T>(new_height - 1, node));
    node->SetRightChild(BuildCompleteTree<T>(new_height - 1, node));
    return node;
}


template<typename T>
class AVLTree {

    AVLNode<T> *root;

    int tree_size;

    AVLNode<T> *RotateRR(AVLNode<T> *node);

    AVLNode<T> *RotateRL(AVLNode<T> *node);

    AVLNode<T> *RotateLL(AVLNode<T> *node);

    AVLNode<T> *RotateLR(AVLNode<T> *node);

    void balanceTree(AVLNode<T> *node);


public:
    AVLTree();

    ~AVLTree();

    void DestroyTree(AVLNode<T> *node);

    AVLTree(const AVLTree &tree) = delete;

    AVLTree &operator=(const AVLTree &tree) = delete;

    int Size() const;

    AVLNode<T> *GetRoot() const {
        return root;
    }

    void SetRoot(AVLNode<T> *new_root) {
        root = new_root;
    }


    void Insert(T &data);//insert a new node to the tree

    //removes a node from the tree and returns the parent - used in Remove function
    AVLNode<T> *RemoveNode(AVLNode<T> *to_remove, AVLNode<T> *child);

    void Remove(T &data);//removes a node from the tree

    AVLNode<T> *Find(T &data);//returns nullptr if the node isn't in the tree

    void InorderReverseTree(T *array) const;

    void InorderTreeFillArray(T *array) const;

    void RemoveLeafsTree(int total_nodes, int required_nodes);

    void InorderTreeFillTree(T *array) const;

    void UpdateNewTree();

};

///helping functions:

template<typename T>
//finds the minimum node in a tree - used in Remove function
AVLNode<T> *FindMin(AVLNode<T> *root) {
    while (root->GetLeftChild() != nullptr) {
        root = root->GetLeftChild();
    }
    return root;
}

template<typename T>
//checks the children "status" for a node in the tree - used in Remove function
ChildrenStatus CheckNodeStatus(AVLNode<T> *node) {
    if (node->GetLeftChild() == nullptr) {
        if (node->GetRightChild() == nullptr) {
            return NO_CHILDREN;
        } else {
            return ONLY_RIGHT_CHILD;
        }
    } else {
        if (node->GetRightChild() == nullptr) {
            return ONLY_LEFT_CHILD;
        } else return HAS_BOTH_CHILDREN;
    }
}



///AVLTree functions start:

template<typename T>
AVLTree<T>::AVLTree(): root(nullptr), tree_size(0) {}

template<typename T>
AVLTree<T>::~AVLTree() {
    DestroyTree(root);
}

template<typename T>
int AVLTree<T>::Size() const {
    return tree_size;
}

template<typename T>
//destroy the entire tree - used in d'tor
void AVLTree<T>::DestroyTree(AVLNode<T> *node) {
    if (node != nullptr) {
        DestroyTree(node->GetLeftChild());
        DestroyTree(node->GetRightChild());
        delete node;
    }
}


template<typename T>
AVLNode<T> *AVLTree<T>::RotateLL(AVLNode<T> *node) {
    AVLNode<T> *parent = node->GetParent();
    AVLNode<T> *temp = node->GetLeftChild();
    if (temp->GetRightChild() != nullptr) {
        node->SetLeftChild(temp->GetRightChild());
        temp->GetRightChild()->SetParent(node);
    } else {
        node->SetLeftChild(nullptr);
    }
    temp->SetRightChild(node);
    node->SetParent(temp);
    temp->SetParent(parent);
    if (parent != nullptr) {
        if (parent->GetLeftChild() == node) {
            parent->SetLeftChild(temp);
        } else parent->SetRightChild(temp);
    } else {
        root = temp;
        temp->SetParent(nullptr);
    }
    node->SetHeight();
    temp->SetHeight();
    return temp;
}

template<typename T>
AVLNode<T> *AVLTree<T>::RotateRR(AVLNode<T> *node) {
    AVLNode<T> *parent = node->GetParent();
    AVLNode<T> *temp = node->GetRightChild();
    if (temp->GetLeftChild() != nullptr) {
        node->SetRightChild(temp->GetLeftChild());
        temp->GetLeftChild()->SetParent(node);
    } else {
        node->SetRightChild(nullptr);
    }
    temp->SetLeftChild(node);
    node->SetParent(temp);
    temp->SetParent(parent);
    if (parent != nullptr) {
        if (parent->GetRightChild() == node) {
            parent->SetRightChild(temp);
        } else parent->SetLeftChild(temp);
    } else {
        root = temp;
        temp->SetParent(nullptr);
    }
    node->SetHeight();
    temp->SetHeight();
    return temp;
}

template<typename T>
AVLNode<T> *AVLTree<T>::RotateLR(AVLNode<T> *node) {
    AVLNode<T> *parent = node->GetParent();
    AVLNode<T> *temp1 = node->GetLeftChild();
    AVLNode<T> *temp2 = temp1->GetRightChild();
    if (temp2->GetRightChild() != nullptr) {
        node->SetLeftChild(temp2->GetRightChild());
        temp2->GetRightChild()->SetParent(node);
    } else node->SetLeftChild(nullptr);
    temp2->SetRightChild(node);
    node->SetParent(temp2);
    if (temp2->GetLeftChild() != nullptr) {
        temp1->SetRightChild(temp2->GetLeftChild());
        temp2->GetLeftChild()->SetParent(temp1);
    } else temp1->SetRightChild(nullptr);
    temp1->SetParent(temp2);
    temp2->SetLeftChild(temp1);
    temp2->SetParent(parent);
    if (parent != nullptr) {
        if (parent->GetRightChild() == node) {
            parent->SetRightChild(temp2);
        } else parent->SetLeftChild(temp2);
    } else {
        root = temp2;
    }
    node->SetHeight();
    temp1->SetHeight();
    temp2->SetHeight();
    return temp2;
}

template<typename T>
AVLNode<T> *AVLTree<T>::RotateRL(AVLNode<T> *node) {
    AVLNode<T> *parent = node->GetParent();
    AVLNode<T> *temp1 = node->GetRightChild();
    AVLNode<T> *temp2 = temp1->GetLeftChild();
    if (temp2->GetLeftChild() != nullptr) {
        node->SetRightChild(temp2->GetLeftChild());
        temp2->GetLeftChild()->SetParent(node);
    } else node->SetRightChild(nullptr);
    temp2->SetLeftChild(node);
    node->SetParent(temp2);
    if (temp2->GetRightChild() != nullptr) {
        temp1->SetLeftChild(temp2->GetRightChild());
        temp2->GetRightChild()->SetParent(temp1);
    } else temp1->SetLeftChild(nullptr);
    temp1->SetParent(temp2);
    temp2->SetRightChild(temp1);
    temp2->SetParent(parent);
    if (parent != nullptr) {
        if (parent->GetRightChild() == node) {
            parent->SetRightChild(temp2);
        } else parent->SetLeftChild(temp2);
    } else {
        root = temp2;
    }
    node->SetHeight();
    temp1->SetHeight();
    temp2->SetHeight();
    return temp2;
}

template<typename T>
void AVLTree<T>::balanceTree(AVLNode<T> *node) {
    if (node == nullptr) {
        return;
    }
    while (true) {
        node->SetHeight();
        if (node->GetBalance() > 1) {
            if (node->GetLeftChild()->GetBalance() >= 0) {
                RotateLL(node);
            } else if (node->GetLeftChild()->GetBalance() == -1) {
                RotateLR(node);
            }
        } else if (node->GetBalance() < -1) {
            if (node->GetRightChild()->GetBalance() <= 0) {
                RotateRR(node);
            } else if (node->GetRightChild()->GetBalance() == 1) {
                RotateRL(node);
            }
        }
        node = node->GetParent();
        if (node == nullptr) {
            break;
        }
    }
}

template<typename T>
void AVLTree<T>::Insert(T &data) {
    auto *new_node = new AVLNode<T>(data);
    if (root == nullptr) { //if the tree is empty
        root = new_node;
        tree_size = 1;
        return;
    }
    bool IsOriginLeft = false;
    AVLNode<T> *current = root;
    AVLNode<T> *parent = root;
    while (current != nullptr) {
        parent = current;
        if (data > current->GetData()) {
            current = current->GetRightChild();
            IsOriginLeft = false;
        } else {
            current = current->GetLeftChild();
            IsOriginLeft = true;
        }
    }
    new_node->SetParent(parent);
    if (IsOriginLeft) {
        parent->SetLeftChild(new_node);
    } else parent->SetRightChild(new_node);
    tree_size++;
    balanceTree(new_node);
}


template<typename T>
AVLNode<T> *AVLTree<T>::RemoveNode(AVLNode<T> *to_remove, AVLNode<T> *child) {
    if (to_remove == root) {
        root = child;
        if (child != nullptr) {
            child->SetParent(nullptr);
        }
        delete to_remove;
        return nullptr;
    }
    AVLNode<T> *parent = to_remove->GetParent();
    if (to_remove == parent->GetLeftChild()) {
        parent->SetLeftChild(child);
    } else if (to_remove == parent->GetRightChild()) {
        parent->SetRightChild(child);
    }
    if (child != nullptr) {
        child->SetParent(parent);
    }
    delete to_remove;
    return parent;
}

template<typename T>
void AVLTree<T>::Remove(T &data) {
    if (root == nullptr) {
        return;
    }
    AVLNode<T> *to_remove = nullptr;
    to_remove = Find(data);
    ChildrenStatus status = CheckNodeStatus(to_remove);
    AVLNode<T> *parent = nullptr;
    if (status == NO_CHILDREN) {
        parent = RemoveNode(to_remove, nullptr);
    } else if (status == ONLY_LEFT_CHILD) {
        parent = RemoveNode(to_remove, to_remove->GetLeftChild());
    } else if (status == ONLY_RIGHT_CHILD) {
        parent = RemoveNode(to_remove, to_remove->GetRightChild());
    } else { // HAS_BOTH_CHILDREN
        AVLNode<T> *to_keep = nullptr;
        to_keep = FindMin(to_remove->GetRightChild());

        AVLNode<T> *original_parent = to_keep->GetParent();
        AVLNode<T> *original_right = to_keep->GetRightChild();

        to_keep->SetParent(to_remove->GetParent());
        if (to_keep->GetParent() != nullptr) {
            if (to_keep->GetParent()->GetRightChild() == to_remove) {
                to_keep->GetParent()->SetRightChild(to_keep);
            } else {
                to_keep->GetParent()->SetLeftChild(to_keep);
            }
        }

        to_keep->SetLeftChild(to_remove->GetLeftChild());
        if (to_keep->GetLeftChild() != nullptr)
            to_keep->GetLeftChild()->SetParent(to_keep);


        if (to_remove->GetRightChild() != to_keep) {
            to_keep->SetRightChild(to_remove->GetRightChild());
            if (to_keep->GetRightChild() != nullptr)
                to_keep->GetRightChild()->SetParent(to_keep);
        } else
            to_keep->SetRightChild(to_remove);

        to_keep->SetHeightWithNum(to_remove->GetHeight());
        if (root == to_remove)
            root = to_keep;

        if (original_parent != to_remove) {
            to_remove->SetParent(original_parent);
            if (to_remove->GetParent() != nullptr) {
                if (to_remove->GetParent()->GetRightChild() == to_keep)
                    to_remove->GetParent()->SetRightChild(to_remove);
                if (to_remove->GetParent()->GetLeftChild() == to_keep)
                    to_remove->GetParent()->SetLeftChild(to_remove);
            }
        } else
            to_remove->SetParent(to_keep);

        to_remove->SetLeftChild(nullptr);

        to_remove->SetRightChild(original_right);
        if (to_remove->GetRightChild() != nullptr) {
            to_remove->GetRightChild()->SetParent(to_remove);
        }

        to_remove->SetHeightWithNum(to_keep->GetHeight());

        status = CheckNodeStatus(to_remove);
        if (status == NO_CHILDREN) {
            parent = RemoveNode(to_remove, nullptr);
        } else {
            parent = RemoveNode(to_remove, to_remove->GetRightChild());
        }
    }
    tree_size--;
    balanceTree(parent);
}

template<typename T>
AVLNode<T> *AVLTree<T>::Find(T &data) {
    AVLNode<T> *current = root;
    while (current != nullptr) {
        if (data == current->GetData()) { //T has a == operator
            return current;
        }
        if (data > current->GetData()) {
            current = current->GetRightChild();
        } else {
            current = current->GetLeftChild();
        }
    }
    return nullptr;
}

template<typename T>
void AVLTree<T>::InorderReverseTree(T *array) const {
    if (root == nullptr) {
        return;
    }
    int counter = 0;
    return root->InorderReverseNode(array, counter);
}

template<typename T>
void AVLTree<T>::InorderTreeFillArray(T *array) const {
    if (root == nullptr) {
        return;
    }
    int counter = 0;
    return root->InorderNodeFillArray(array, counter);
}

template<typename T>
void AVLTree<T>::RemoveLeafsTree(int total_nodes, int required_nodes) {
    if (root == nullptr) {
        return;
    }
    int to_delete = total_nodes - required_nodes;
    root->DeleteLeafs(to_delete);
    }

template<typename T>
void AVLTree<T>::InorderTreeFillTree(T *array) const {
    if (root == nullptr) {
        return;
    }
    int counter = 0;
    return root->InorderNodeFillTree(array, counter);
}

template<typename T>
void AVLTree<T>::UpdateNewTree() {
    if (root == nullptr) {
        return;
    }
    return root->UpdateNode(root);
}

#endif //WET2_AVLTREE_H
