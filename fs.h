//
// Created by pc on 5/30/2024.
//

#ifndef ESTIMATINGSTRINGPATTERNALGORITHMS_FS_H
#define ESTIMATINGSTRINGPATTERNALGORITHMS_FS_H
#include "include/define.h"
#include "include/main_header.h"
#include "Algorithm.h"

class fs : public Algorithm {
public:
    const char* name() const override {
        return "FS";
    }
    void Pre_GS(unsigned char *x, int m, int bm_gs[]) {
        int i, j, p, f[XSIZE];
        for(i=0;i<XSIZE;i++) bm_gs[i]=0;
        f[m]=j=m+1;
        for (i=m; i > 0; i--) {
            while (j <= m && x[i-1] != x[j-1]) {
                if (bm_gs[j] == 0) bm_gs[j]=j-i;
                j=f[j];
            }
            f[i-1]=--j;
        }
        p=f[0];
        for (j=0; j <= m; ++j) {
            if (bm_gs[j] == 0) bm_gs[j]=p;
            if (j == p) p=f[p];
        }
    }
    int search(unsigned char *x, int m, unsigned char *y, int n) override {
        int a,i, j, k, s, count, bc[SIGMA], gs[XSIZE];
        char ch = x[m-1];

        /* Preprocessing */
        for (a=0; a < SIGMA; a++) bc[a]=m;
        for (j=0; j < m; j++) bc[x[j]]=m-j-1;
        Pre_GS(x, m, gs);
        for(i=0; i<m; i++) y[n+i]=ch;

        /* Searching */
        count = 0;
        if( !memcmp(x,y,m) ) count++;
        s=m;
        while(s<n) {
            while((k=bc[y[s]]))   s += k;
            j=2;
            while (j<=m && x[m-j]==y[s-j+1]) j++;
            if( j>m && s<n) count++;
            s += gs[m-j+1];
        }
        return count;
    }
};

#endif //ESTIMATINGSTRINGPATTERNALGORITHMS_FS_H
