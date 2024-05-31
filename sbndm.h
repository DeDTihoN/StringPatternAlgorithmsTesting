//
// Created by pc on 5/31/2024.
//

#ifndef ESTIMATINGSTRINGPATTERNALGORITHMS_SBNDM_H
#define ESTIMATINGSTRINGPATTERNALGORITHMS_SBNDM_H

#include "include/define.h"
#include "include/main_header.h"
#include "Algorithm.h"

class sbndm : public Algorithm {
public:
    const char *name() const override {
        return "SBNDM";
    }

    int search(unsigned char *x, int m, unsigned char *y, int n) override {
        int j, i, last, first;
        unsigned int D, B[SIGMA], s;
        int mM1 = m - 1;
        int mM2 = m - 2;
        int count = 0, restore[XSIZE + 1], shift;

        if (m > 32) return search_large(x, m, y, n);
        if (m < 2) {
            KMP kmp;
            return kmp.search(x, m, y, n);
        }

        /* Preprocessing */
        for (i = 0; i < SIGMA; i++) B[i] = 0;
        for (i = 0; i < m; i++) B[x[m - i - 1]] |= (unsigned int) 1 << (i + WORD - m);

        last = m;
        s = (unsigned int) (~0) << (WORD - m);
        s = (unsigned int) (~0);
        for (i = m - 1; i >= 0; i--) {
            s &= B[x[i]];
            if (s & ((unsigned int) 1 << (WORD - 1))) {
                if (i > 0) last = i;
            }
            restore[i] = last;
            s <<= 1;
        }
        shift = restore[0];

        for (i = 0; i < m; i++) y[n + i] = x[i];

        /* Searching */
        if (!memcmp(x, y, m)) OUTPUT(0);
        i = m;
        while (1) {
            D = B[y[i]];
            j = i - 1;
            first = i - m + 1;
            while (1) {
                D = (D << 1) & B[y[j]];
                if (!((j - first) && D)) break;
                j--;
            }
            if (D != 0) {
                if (i >= n) {
                    return count;
                }
                OUTPUT(first);
                i += shift;
            } else {
                i = j + m;
            }
        }
    }

/*
 * Simplified Backward Nondeterministic DAWG Matching algorithm designed for large patterns
 * The present implementation searches for prefixes of the pattern of length 32.
 * When an occurrence is found the algorithm tests for the whole occurrence of the pattern
 */

    int search_large(unsigned char *x, int m, unsigned char *y, int n) {
        int j, i, last, first, p_len, k;
        unsigned int D, B[SIGMA], s;
        int mM1 = m - 1;
        int mM2 = m - 2;
        int count = 0, restore[XSIZE + 1], shift;

        p_len = m;
        m = 32;
        int diff = p_len - m;

        /* Preprocessing */
        for (i = 0; i < SIGMA; i++) B[i] = 0;
        for (i = 0; i < m; i++) B[x[m - i - 1]] |= (unsigned int) 1 << (i + WORD - m);

        last = m;
        s = (unsigned int) (~0) << (WORD - m);
        s = (unsigned int) (~0);
        for (i = m - 1; i >= 0; i--) {
            s &= B[x[i]];
            if (s & ((unsigned int) 1 << (WORD - 1))) {
                if (i > 0) last = i;
            }
            restore[i] = last;
            s <<= 1;
        }
        shift = restore[0];

        for (i = 0; i < m; i++) y[n + i] = x[i];

        /* Searching */
        if (!memcmp(x, y, p_len)) OUTPUT(0);
        i = m;
        while (1) {
            while ((D = B[y[i]]) == 0) i += m;
            j = i - 1;
            first = i - m + 1;
            while (1) {
                D = (D << 1) & B[y[j]];
                if (!((j - first) && D)) break;
                j--;
            }
            if (D != 0) {
                if (i + diff >= n) {
                    return count;
                }
                k = m;
                while (k < p_len && x[k] == y[first + k]) k++;
                if (k == p_len) OUTPUT(first);
                i += shift;
            } else {
                i = j + m;
            }
        }
    }
};

#endif //ESTIMATINGSTRINGPATTERNALGORITHMS_SBNDM_H
