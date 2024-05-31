//
// Created by pc on 5/31/2024.
//

#ifndef ESTIMATINGSTRINGPATTERNALGORITHMS_BSDM_H
#define ESTIMATINGSTRINGPATTERNALGORITHMS_BSDM_H

#include "include/define.h"
#include "include/main_header.h"
#include "Algorithm.h"

class bsdm : public Algorithm {
    public:
    const char* name() const override {
        return "BSDM";
    }
    int search(unsigned char *x, int m, unsigned char *y, int n) override {
        unsigned int B[SIGMA];
        int i, j, k, count;
        unsigned int s,d;
        unsigned int occ[SIGMA] = {0};
        int st = 0, len = 0;
        for(i=0, j=0; i<m; i++) {
            if(occ[x[i]]) {
                while(x[j]!=x[i]) {
                    occ[x[j]]=0;
                    j++;
                }
                occ[x[j]]=0;
                j++;
            }
            occ[x[i]]=1;
            if(len < i-j+1 ) {
                len = i-j+1;
                st = j;
            }
        }
        unsigned int pos[SIGMA];
        for(i=0; i<SIGMA; i++) pos[i]=-1;
        for(i=0; i<len; i++) pos[x[st+i]]=i;

        count = 0;
        for(i=0; i<m; i++) y[n+i]=x[i];
        unsigned char *xst = x+st;
        int offset = st+len-1;
        j = st+len-1;
        while(j<n) {
            while ((i=pos[y[j]])<0) j+=len; //fast loop
            //printf("%c - %c\n",y[j],x[st+i]);
            k=1;
            while(k<=i && xst[i-k]==y[j-k]) k++;
            if(k>i) { // a suffix has been found
                if(k==len) {
                    if(!memcmp(x,y+j-offset,m)) if(j-offset<=n-m) count++;
                }
                else j-=k;
            }
            j+=len;
        }
        return count;
    }
};

#endif //ESTIMATINGSTRINGPATTERNALGORITHMS_BSDM_H
