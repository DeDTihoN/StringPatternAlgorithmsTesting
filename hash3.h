//
// Created by pc on 5/30/2024.
//

#ifndef ESTIMATINGSTRINGPATTERNALGORITHMS_HASH3_H
#define ESTIMATINGSTRINGPATTERNALGORITHMS_HASH3_H

#include "include/define.h"
#include "include/main_header.h"
#include "Algorithm.h"
#define RANK3 3


class hash3 : public Algorithm {
public:
    const char *name() const override {
        return "Hash3";
    }

    int search(unsigned char *x, int m, unsigned char *y, int n) override {
        int count, j, i, sh, sh1, mMinus1, mMinus2, shift[WSIZE];
        unsigned char h;
        if (m < 3){
            KMP kmp;
            return kmp.search(x, m, y, n);
        }
        count = 0;
        mMinus1 = m - 1;
        mMinus2 = m - 2;

        /* Preprocessing */
        for (i = 0; i < WSIZE; ++i)
            shift[i] = mMinus2;

        h = x[0];
        h = ((h << 1) + x[1]);
        h = ((h << 1) + x[2]);
        shift[h] = m - RANK3;
        for (i = RANK3; i < mMinus1; ++i) {
            h = x[i - 2];
            h = ((h << 1) + x[i - 1]);
            h = ((h << 1) + x[i]);
            shift[h] = mMinus1 - i;
        }
        h = x[i - 2];
        h = ((h << 1) + x[i - 1]);
        h = ((h << 1) + x[i]);
        sh1 = shift[h];
        shift[h] = 0;
        if (sh1 == 0) sh1 = 1;


        /* Searching */
        i = mMinus1;
        memcpy(y + n, x, m);
        while (1) {
            sh = 1;
            while (sh != 0) {
                h = y[i - 2];
                h = ((h << 1) + y[i - 1]);
                h = ((h << 1) + y[i]);
                sh = shift[h];
                i += sh;
            }
            if (i < n) {
                j = 0;
                while (j < m && x[j] == y[i - mMinus1 + j]) j++;
                if (j >= m) {
                    OUTPUT(i - mMinus1);
                }
                i += sh1;
            } else return count;
        }
    }
};
#endif //ESTIMATINGSTRINGPATTERNALGORITHMS_HASH3_H
