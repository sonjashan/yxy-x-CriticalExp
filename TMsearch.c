// use the makefile, need fns in the header

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "powerfree.h"

// this is DFS
// for pre morphism sequences from 2 letter alphabets like TM
// xLen and yLen are the min length as in avoid_yxyprime()
// ltrMLen is the max |h(0)| of the morphism we are looking for
// return 0 not found, 1 found, -1 error
int backtrack_search(int pre[], int preLen, int yLen, int xLen, int n, int p, int plus, int ltrMLen){
    int psCount = 4;            // tm uses a 2 letter alphabet and h of a, b each has a pre- and suffix

    int maxMLen = ltrMLen * 2;     // morphism is an array that includes h(0), h(1), see prefix suffix search.pdf
    int morphism[maxMLen];
    int i = 0; 
    morphism[i] = 0;

    int l = ceiling(ltrMLen, 2);                        // pre- or suffix h(a) or h(0) max length
    int pa[l], pb[l], sa[l], sb[l];       // pa is prefix of h(a)
    int ab[ltrMLen], ba[ltrMLen], aa[ltrMLen], bb[ltrMLen];       // suffix of a concatenated with prefix of b, etc.

    unsigned int count = 0;
    time_t start, now;
    start = time(NULL);
    FILE *fp;
    fp = fopen("TMyxyprimex.txt", "a");   // could add checks for error opening file
    fprintf(fp, "ltrMLen: %d\n", ltrMLen);
    fclose(fp);

    while(i < maxMLen){
        // printf("************************\n");
        // printf("morphism: ");
        // printIntArray(morphism, i + 1, 0);
        int extend = 0;
        int backtrack = 1;

        int hLen = i + 1;                       // the morphism length so far
        int paIdx = 0, pbIdx = 0, saIdx = 0, sbIdx = 0;
        int paLen = 0, pbLen = 0, saLen = 0, sbLen = 0;       // need the precise length for use later

        // pa and such is at least 1/6 of maxMLen which is the max value of hLen, so should not run out of space
        for(int j = 0; j < hLen; j++){
            if(j % psCount == 0){
                pa[paIdx] = morphism[j];
                paIdx++;
                paLen++;
            } else if(j % psCount == 1){
                pb[pbIdx] = morphism[j];
                pbIdx++;
                pbLen++;
            } else if(j % psCount == 2){
                sa[saIdx] = morphism[j];
                saIdx++;
                saLen++;
            } else if(j % psCount == 3){
                sb[sbIdx] = morphism[j];
                sbIdx++;
                sbLen++;
            } else {
                printf("ERROR: Did you change psCount without changing the loop that constructs the suffixes and predfixes?\n");
            }
        }
        reverse(sa, saLen);
        reverse(sb, sbLen);

        // printf("before concat pa: ");
        // printIntArray(pa, paLen, 0);
        // printf("before concat pb: ");
        // printIntArray(pb, pbLen, 0);
        // printf("before concat sa: ");
        // printIntArray(sa, saLen, 0);
        // printf("before concat sb: ");
        // printIntArray(sb, sbLen, 0);

        // tm is olapfree not squarefree
        concat(sa, saLen, pb, pbLen, ab);
        concat(sb, sbLen, pa, paLen, ba);
        concat(sa, saLen, pa, paLen, aa);
        concat(sb, sbLen, pb, pbLen, bb);

        // printf("ab: ");
        // printIntArray(ab, saLen + pbLen, 0);
        // printf("ba: ");
        // printIntArray(ba, sbLen + paLen, 0);

        if(avoid_yxyprimex(ab, saLen + pbLen, yLen, xLen) &&
        n_p_powerfree(ab, saLen + pbLen, n, p, plus) &&
        avoid_yxyprimex(ba, sbLen + paLen, yLen, xLen) &&
        n_p_powerfree(ba, sbLen + paLen, n, p, plus) &&
        avoid_yxyprimex(aa, saLen + paLen, yLen, xLen) &&
        n_p_powerfree(aa, saLen + paLen, n, p, plus) &&
        avoid_yxyprimex(bb, sbLen + pbLen, yLen, xLen) &&
        n_p_powerfree(bb, sbLen + pbLen, n, p, plus)){
            backtrack = 0;
            extend = 1;

            // Walnut can only deal with uniform morphisms
            if(hLen % 2 == 0){
                int h0Len = paLen + saLen;
                int h0[h0Len]; 
                int h1[h0Len]; 
                concat(pa, paLen, sa, saLen, h0);
                concat(pb, pbLen, sb, sbLen, h1);

                int postMorphLen = h0Len * preLen;
                int postMorph[postMorphLen];
                apply_bin_morph(pre, preLen, h0, h1, h0Len, postMorph);

                if(avoid_yxyprimex(postMorph, postMorphLen, yLen, xLen) &&
                n_p_powerfree(postMorph, postMorphLen, n, p, plus)){

                    fp = fopen("TMyxyprimex.txt", "a");   // could add checks for error opening file
                    fprintf(fp, "0->");
                    filePrintIntArray(fp, h0, h0Len, 0);
                    fprintf(fp, "1->");
                    filePrintIntArray(fp, h1, h0Len, 0);
                    fclose(fp);
                    
                    // extend = 0;
                    // backtrack = 1;
                    return 1;

                } else if(i == maxMLen - 1){
                    extend = 0;
                    backtrack = 1;
                }
            }
        }        
        if(extend){
            // printf("EXTEND\n");
            // check for i above 
            // if we are here, i < maxMLen - 1
            i++; 
            morphism[i] = 0;
        } 
        if(backtrack){
            // printf("BACKTRACK\n");
            if(morphism[i] == 0) morphism[i] = 1;
            else { 
                while(morphism[i] == 1){
                    i--; 
                    if(i == 0) return 0;
                }
                morphism[i] = 1;
            }
        }
        count++;
        if(count % 10000 == 0){
            now = time(NULL);
            fp = fopen("TMyxyprimex.txt", "a");   // could add checks for error opening file
            fprintf(fp, "checked %d potential sequences in %ld seconds\n", count, now - start);
            fclose(fp);
        }
    }
    printf("ERROR: i should never >= maxMLen, as i only advance in if(extend)\n");
    return -1;
}

int main(){
    // build TM
    static int tmLen = 60;
    int tm[tmLen];
    tm[0] = 0; tm[1] = 1;
    for(int x = 2; x < tmLen; x++) tm[x] = x&1 ? !tm[x-1] : tm[x/2];
    // printIntArray(tm, tmLen);

    int yLen = 2;
    int xLen = 2;
    int n = 5;
    int p = 2;
    int plus = 1;
    int ltrMLen = 30;
    int res = backtrack_search(tm, tmLen, yLen, xLen, n, p, plus, ltrMLen);

    while(res == 0){
        ltrMLen += 5;
        res = backtrack_search(tm, tmLen, yLen, xLen, n, p, plus, ltrMLen);
    }

    // printf("backtrack search found result? %d\n", res);

}

