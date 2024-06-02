//
// Created by pc on 5/30/2024.
//

#ifndef ESTIMATINGSTRINGPATTERNALGORITHMS_BOM_H
#define ESTIMATINGSTRINGPATTERNALGORITHMS_BOM_H

#endif //ESTIMATINGSTRINGPATTERNALGORITHMS_BOM_H

#include "include/define.h"
#include "include/main_header.h"
#include "include/GRAPH.h"
#include "Algorithm.h"
#include "include/cell.h"

class bom : public Algorithm {
    public:
    const char* name() const override {
        return "BOM";
    }
    int getTransition(unsigned char *x, int p, List L[], unsigned char c) {
        List cell;
        if (p > 0 && x[p - 1] == c) return(p - 1);
        else {
            cell = L[p];
            while (cell != NULL)
                if (x[cell->element] == c)
                    return(cell->element);
                else
                    cell = cell->next;
            return(UNDEFINED);
        }
    }

    void setTransition(int p, int q, List L[]) {
        List cell;
        cell = (List)malloc(sizeof(struct _cell));
        if (cell == NULL)
            error("BOM/setTransition");
        cell->element = q;
        cell->next = L[p];
        L[p] = cell;
    }

    void oracle(unsigned char *x, int m, char T[], List L[]) {
        int i, p, q;
        int S[XSIZE + 1];
        char c;
        S[m] = m + 1;
        for (i = m; i > 0; --i) {
            c = x[i - 1];
            p = S[i];
            while (p <= m &&
                   (q = getTransition(x, p, L, c)) ==
                   UNDEFINED) {
                setTransition(p, i - 1, L);
                p = S[p];
            }
            S[i - 1] = (p == m + 1 ? m : q);
        }
        p = 0;
        while (p <= m) {
            T[p] = TRUE;
            p = S[p];
        }
    }

    int search(unsigned char *x, int m, unsigned char *y, int n) override {
        char T[XSIZE + 1];
        List L[XSIZE + 1];
        int i, j, p, period, q, shift, count;

        /* Preprocessing */
        memset(L, 0, (m + 1)*sizeof(List));
        memset(T, FALSE, (m + 1)*sizeof(char));
        oracle(x, m, T, L);

        /* Searching */
        count = 0;
        j = 0;
        while (j <= n - m) {
            i = m - 1;
            p = m;
            shift = m;
            while (i + j >= 0 && (q = getTransition(x, p, L, y[i + j])) != UNDEFINED) {
                p = q;
                if (T[p] == TRUE) {
                    period = shift;
                    shift = i;
                }
                --i;
            }
            if (i < 0) {
                OUTPUT(j);
                shift = period;
            }
            j += shift;
        }
        for(i=0; i<=m; i++) free(L[i]);
        return count;
    }
};