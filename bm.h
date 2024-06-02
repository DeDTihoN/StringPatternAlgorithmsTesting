//
// Created by pc on 5/30/2024.
//

#ifndef ESTIMATINGSTRINGPATTERNALGORITHMS_BM_H
#define ESTIMATINGSTRINGPATTERNALGORITHMS_BM_H

#include "include/define.h"
#include "include/main_header.h"
#include "Algorithm.h"

class bm : public Algorithm {
    public:
    void preBmBc(unsigned char *x, int m, int bmBc[]) {
        int i;
        for (i = 0; i < SIGMA; ++i)
            bmBc[i] = m;
        for (i = 0; i < m - 1; ++i)
            bmBc[x[i]] = m - i - 1;
    }


    void suffixes(unsigned char *x, int m, int *suff) {
        int f, g, i;
        suff[m - 1] = m;
        g = m - 1;
        for (i = m - 2; i >= 0; --i) {
            if (i > g && suff[i + m - 1 - f] < i - g)
                suff[i] = suff[i + m - 1 - f];
            else {
                if (i < g)
                    g = i;
                f = i;
                while (g >= 0 && x[g] == x[g + m - 1 - f])
                    --g;
                suff[i] = f - g;
            }
        }
    }

    void preBmGs(unsigned char *x, int m, int bmGs[]) {
        int i, j, suff[XSIZE];
        suffixes(x, m, suff);
        for (i = 0; i < m; ++i) bmGs[i] = m;
        j = 0;
        for (i = m - 1; i >= 0; --i)
            if (suff[i] == i + 1)
                for (; j < m - 1 - i; ++j)
                    if (bmGs[j] == m)
                        bmGs[j] = m - 1 - i;
        for (i = 0; i <= m - 2; ++i)
            bmGs[m - 1 - suff[i]] = m - 1 - i;
    }
    const char* name() const override {
        return "BM";
    }
    int search(unsigned char *x, int m, unsigned char *y, int n) override {
        int i, j, bmGs[XSIZE], bmBc[SIGMA], count;

        /* Preprocessing */
        preBmGs(x, m, bmGs);
        preBmBc(x, m, bmBc);

        /* Searching */
        j = 0;
        count = 0;
        while (j <= n - m) {
        for (i = m - 1; i >= 0 && x[i] == y[i + j]; --i);
        if (i < 0) {
        OUTPUT(j);
        j += bmGs[0];
        }
        else
        j += MAX(bmGs[i], bmBc[y[i + j]] - m + 1 + i);
        }
        return count;
    }
};

#endif //ESTIMATINGSTRINGPATTERNALGORITHMS_BM_H
