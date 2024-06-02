//
// Created by pc on 6/1/2024.
//

#ifndef ESTIMATINGSTRINGPATTERNALGORITHMS_SSEF_H
#define ESTIMATINGSTRINGPATTERNALGORITHMS_SSEF_H

#include <emmintrin.h>
#include "include/define.h"
#include "include/main_header.h"
#include "Algorithm.h"

typedef union{
    __m128i* data16;
    unsigned char* data;
} TEXT;

typedef struct list {
    struct list *next;
    int pos;
} LIST;

class SSEF : public Algorithm {
public:
    const char* name() const override {
        return "SSEF";
    }
    int search(unsigned char *x, int Plen, unsigned char *y, int Tlen) override {
        if(Plen<32) {
            KMP kmp;
            return kmp.search(x,Plen,y,Tlen);
        }
        LIST *flist[65536];
        LIST *t;
        memset(flist,0,sizeof(LIST*)*65536);
        __m128i tmp128;
        TEXT T;
        T.data16 = (__m128i*) y;
        T.data = (unsigned char *) y;

        unsigned int K=7;
        unsigned int count=0;
        unsigned int i,last,j;
        __m128i *ptr16;
        __m128i *lastchunk = &T.data16[Tlen/16];
        unsigned int filter;
        unsigned char* f = static_cast<unsigned char *>(malloc(Plen));

        last = (Plen/16) - 1;
        for(i=0;i<Plen;i++){
            f[i] = (x[i]&0x80)>>7;
        }
        count = 15;

        for(i=0;i<last*16;i++) {
            j = last*16-i;
            filter = f[j] + f[j+1]*2     + f[j+2]*4     + f[j+3]*8      +
                     f[j+4]*16    + f[j+5]*32    + f[j+6]*64     + f[j+7]*128    +
                     f[j+8]*256   + f[j+9]*512   + f[j+10]*1024  + f[j+11]*2048  +
                     f[j+12]*4096 + f[j+13]*8192 + f[j+14]*16384 + f[j+15]*32768 ;
            if (flist[filter]==0){
                flist[filter] = (LIST*)malloc(sizeof(LIST));
                flist[filter]->next = NULL;
                flist[filter]->pos  = i;
            } else {
                t = flist[filter];
                while(t->next!=NULL) t = t->next;
                t->next = (LIST*)malloc(sizeof(LIST));
                t = t->next;
                t->next=NULL;
                t->pos = i;
            }
        }

        count=0;
        ptr16 = &T.data16[last];

        while(ptr16<lastchunk) {
            filter = _mm_movemask_epi8(*ptr16);

            if (flist[filter]) {
                i = ((ptr16 - &T.data16[0])-last)*16;
                t = flist[filter];
                while(t) {
                    if (memcmp(x,&T.data[i+t->pos],Plen) == 0) count++;
                    t=t->next;
                }
            }
            ptr16+=last;
        }
        return count;
    }
};

#endif //ESTIMATINGSTRINGPATTERNALGORITHMS_SSEF_H
