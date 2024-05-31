//
// Created by pc on 5/30/2024.
//

#ifndef ESTIMATINGSTRINGPATTERNALGORITHMS_SKIP_H
#define ESTIMATINGSTRINGPATTERNALGORITHMS_SKIP_H

#include "include/define.h"
#include "include/main_header.h"
#include "include/AUTOMATON.h"
#include "Algorithm.h"
#include "bm.h"

class skip : public Algorithm {
public:
    const char* name() const override {
        return "skip";
    }
    int search(unsigned char *x, int m, unsigned char *y, int n) override {
        int i, j, count, h, k;
        List ptr, z[SIGMA];
        memset(z, 0, SIGMA*sizeof(List));
        for (i = 0; i < m; ++i) {
            ptr = (List)malloc(sizeof(struct _cell));
            if (ptr == NULL)
                error("SKIP");
            ptr->element = i;
            ptr->next = z[x[i]];
            z[x[i]] = ptr;
        }
        count = 0;
        /* Searching */
        for (j = m - 1; j < n; j += m)
            for (ptr = z[y[j]]; ptr != NULL; ptr = ptr->next)
                if((j-ptr->element) <= n-m) {
                    k = 0;
                    h = j-ptr->element;
                    while(k<m && x[k]==y[h+k]) k++;
                    if(k>=m) count++;
                }
        return count;
    }
};

#endif //ESTIMATINGSTRINGPATTERNALGORITHMS_SKIP_H
