#ifndef WET2_UF_H
#define WET2_UF_H

#include "DataCenter.h"

#define NO_PARENT -1
#define NO_DATA_CENTER -1

class UF {
    int *parents_arr;
    int *sizes_arr;
    DataCenter *data_centers;


public:
    explicit UF(int arr_size);

    ~UF();

    UF(const UF &other) = delete;

    UF &operator=(const UF &other) = delete;

    int* GetParentsArray() const;

    int* GetSizesArray() const;

    DataCenter* GetDataCenters() const;

    int findRoot(int index);

    int Find(int index);

    int Union(int index1, int index2); //returns the index of the new union (index 1 or 2 according to sizes)

};


#endif //WET2_UF_H
