//
// Created by pc on 5/31/2024.
//

#ifndef ESTIMATINGSTRINGPATTERNALGORITHMS_CELL_H
#define ESTIMATINGSTRINGPATTERNALGORITHMS_CELL_H

struct _cell{
    int element;
    struct _cell *next;
};

typedef struct _cell *List;

#endif //ESTIMATINGSTRINGPATTERNALGORITHMS_CELL_H
