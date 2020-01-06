#include "UF.h"

UF::UF(int arr_size) {
    parents_arr = new int[arr_size + 1]; //first space in array isnt used
    parents_arr[0] = NO_DATA_CENTER;
    for (int i = 1; i < arr_size + 1; i++) {
        parents_arr[i] = NO_PARENT;
    }
    sizes_arr = new int[arr_size + 1];  //first space in array isnt used
    sizes_arr[0] = NO_DATA_CENTER;
    for (int i = 1; i < arr_size + 1; i++) {
        sizes_arr[i] = 1;
    }
    data_centers = new DataCenter[arr_size + 1];
    for (int i = 1; i < arr_size + 1; i++) {
        data_centers[i].SetId(i);
    }
}

UF::~UF() {
    delete[] parents_arr;
    delete[] sizes_arr;
    delete[] data_centers;
}

int *UF::GetParentsArray() const {
    return parents_arr;
}

int *UF::GetSizesArray() const {
    return sizes_arr;
}

DataCenter *UF::GetDataCenters() const {
    return data_centers;
}

int UF::findRoot(int index) {
    while (parents_arr[index] != NO_PARENT) {
        index = parents_arr[index];
    }
    return index;
}

int UF::Find(int index) {
    int root = findRoot(index);
    int temp = 0;
    while (parents_arr[index] != NO_PARENT) { //path shrink
        temp = parents_arr[index];
        parents_arr[index] = root;
        index = temp;
    }
    return root;
}

int UF::Union(int index1, int index2) {
    int root1 = Find(index1);
    int root2 = Find(index2);
    if(root1 == root2){
        return root1;
    }
    if(sizes_arr[root1] >= sizes_arr[root2]){ //union the small group with the large one
        parents_arr[root2] = root1;
        sizes_arr[root1] += sizes_arr[root2];
        sizes_arr[root2] = 0;
        return root1;
    }
    else{
        parents_arr[root1] = root2; //union the small group with the large one
        sizes_arr[root2] += sizes_arr[root1];
        sizes_arr[root1] = 0;
    }
    return root2;
}

