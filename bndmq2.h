//
// Created by pc on 5/30/2024.
//

#ifndef ESTIMATINGSTRINGPATTERNALGORITHMS_BNDMQ2_H
#define ESTIMATINGSTRINGPATTERNALGORITHMS_BNDMQ2_H

#include "include/define.h"
#include "include/main_header.h"
#include "Algorithm.h"
#define GRAM2(i) (B[y[i+1]]<<1)&B[y[i]]

/*
 * Simplified Backward Nondeterministic DAWG Matching algorithm designed for large patterns
 * The present implementation searches for prefixes of the pattern of length 32.
 * When an occurrence is found the algorithm tests for the whole occurrence of the pattern
 */


class bndmq2 : public Algorithm {
    public:
    const char* name() const override {
        return "BNDMQ2";
    }
    int search(unsigned char *x, int m, unsigned char *y, int n) {
        unsigned int D, B[SIGMA], M, s;
        int i,j,q, count, first;
        q = 2;
        if(m<q) {
            KMP kmp;
            return kmp.search(x,m,y,n);
        }
        if(m>WORD) return search_large(x,m,y,n);

        /* Preprocessing */
        for (i=0; i<SIGMA; i++) B[i] = 0;
        s=1;
        for (i=m-1; i>=0; i--){
            B[x[i]] |= s;
            s <<= 1;
        }
        for (j=0; j<m; j++) y[n+j]=x[j];
        M = 1 << (m-1);

        /* Searching */
        count = 0;
        if(!memcmp(x,y,m)) OUTPUT(0);
        i = m+1-q;
        while (i <= n - q) {
            D = GRAM2(i);
            if (D != 0) {
                j = i;
                first = i - (m - q);
                do {
                    if ( D >= M ) {
                        if (j > first) i = j-1;
                        else OUTPUT(first);
                    }
                    j = j-1;
                    D = (D<<1) & B[y[j]];
                } while (D != 0);
            }
            i = i+m-q+1;
        }
        return count;
    }

/*
 * Backward Nondeterministic DAWG Matching using q-grams designed for large patterns
 * The present implementation searches for prefixes of the pattern of length 32.
 * When an occurrence is found the algorithm tests for the whole occurrence of the pattern
 */

    int search_large(unsigned char *x, int m, unsigned char *y, int n) {
        unsigned int D, B[SIGMA], M, s;
        int i,j,q, count, first, p_len, k;
        q = 2;
        if(m<=q) return 0;

        p_len = m;
        m = 32;

        /* Preprocessing */
        for (i=0; i<SIGMA; i++) B[i] = 0;
        s=1;
        for (i=m-1; i>=0; i--){
            B[x[i]] |= s;
            s <<= 1;
        }
        for (j=0; j<m; j++) y[n+j]=x[j];
        M = 1 << (m-1);

        /* Searching */
        count = 0;
        if(!memcmp(x,y,p_len)) OUTPUT(0);
        i = m+1-q;
        while (i <= n - q) {
            D = GRAM2(i);
            if (D != 0) {
                j = i;
                first = i - (m - q);
                do {
                    if ( D >= M ) {
                        if (j > first) i = j-1;
                        else {
                            if(first <= n-p_len && !memcmp(x,y+first,p_len)) OUTPUT(first);
                        }
                    }
                    j = j-1;
                    D = (D<<1) & B[y[j]];
                } while (D != 0);
            }
            i = i+m-q+1;
        }
        return count;
    }
};

#endif //ESTIMATINGSTRINGPATTERNALGORITHMS_BNDMQ2_H
