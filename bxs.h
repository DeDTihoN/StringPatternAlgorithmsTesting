//
// Created by pc on 5/30/2024.
//

#ifndef ESTIMATINGSTRINGPATTERNALGORITHMS_BXS_H
#define ESTIMATINGSTRINGPATTERNALGORITHMS_BXS_H

#endif //ESTIMATINGSTRINGPATTERNALGORITHMS_BXS_H

#include "Algorithm.h"
#include "include/define.h"
#define Q 1

class bxs : public Algorithm {
public:
    const char* name() const override {
        return "BXS";
    }
    int search(unsigned char *x, int m, unsigned char *y, int n) override {
        unsigned int B[SIGMA], D, set;
        int i, j, first, k, mm, sh, m1, count;
        if (m < Q) return -1;
        if (m>128){
            KMP kmp;
            return kmp.search(x, m, y, n);
        }
        //int larger = m>WORD? 1:0;
        //if(larger) m = WORD;
        int w = WORD, mq1 = m - Q + 1, nq1 = n - Q + 1;
        if (w > m) w = m;
        unsigned int mask = 1 << (w - 1);

        /* Preprocessing */
        count = 0;
        set = 1;
        for (i = 0; i < SIGMA; i++) B[i] = 0;
        for (i = m - 1; i >= 0; i--) {
            B[x[i]] |= set;
            set <<= 1;
            if (set == 0) set = 1;
        }

        /* Searching */
        for (i = mq1 - 1; i < nq1; i += mq1) {
            D = B[y[i]];
            if (D) {
                j = i;
                first = i - mq1;
                do {
                    j--;
                    if (D >= mask) {
                        if (j - first) i = j;
                        else {
                            for (k = m; y[first + k] == x[k - 1] && (k); k--);
                            if (k == 0) count++;
                        }
                        D = ((D << 1) | 1) & B[y[j]];
                    } else D = (D << 1) & B[y[j]];
                } while (D && j > first);
            }
        }
        return count;
    }
};