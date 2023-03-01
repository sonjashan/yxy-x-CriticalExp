// can only be nonuniform if |h(02)| = 2 * |h(1)|

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "powerfree.h"


// this is DFS
// for pre morphism sequences from 3 letter alphabets like vtm
// xLen and yLen are the min length as in avoid_yxyprime()
// ltrMLen is the max |h(0)| of the morphism we are looking for
// return 0 not found, 1 found, -1 error
int backtrack_search(int pre[], int preLen, int yLen, int xLen, int n, int p, int plus, int ltrMLen){
    int psCount = 6;            // vtm uses a 3 letter alphabet and h of a, b, c each has a pre- and suffixes

    int maxMLen = ltrMLen * 3;     // morphism is an array that includes h(0), h(1), and h(2), see prefix suffix search.pdf
    int morphism[maxMLen];
    int i = 0; 
    morphism[i] = 0;

    int l = ceiling(ltrMLen, 2);                        // pre- or suffix h(a) or h(0) max length
    int pa[l], pb[l], pc[l], sa[l], sb[l], sc[l];       // pa is prefix of h(a)
    int ab[ltrMLen], ac[ltrMLen], ba[ltrMLen], bc[ltrMLen], ca[ltrMLen], cb[ltrMLen];       // suffix of a concatenated with prefix of b, etc.

    unsigned int count = 0;
    time_t start, now;
    start = time(NULL);
    FILE *fp;
    fp = fopen("nonuniVTMyxyprimex.txt", "a");   // could add checks for error opening file
    fprintf(fp, "ltrMLen: %d\n", ltrMLen);
    fclose(fp);

    while(i < maxMLen){
        // printf("************************\n");
        // printf("morphism: ");
        // printIntArray(morphism, i + 1, 0);
        int extend = 0;
        int backtrack = 1;

        int HLen = i + 1;                       // the morphism length so far
        int paIdx = 0, pbIdx = 0, pcIdx = 0, saIdx = 0, sbIdx = 0, scIdx = 0;
        int paLen = 0, pbLen = 0, pcLen = 0, saLen = 0, sbLen = 0, scLen = 0;       // need the precise length for use later

        // pa and such is at least 1/6 of maxMLen which is the max value of HLen, so should not run out of space
        for(int j = 0; j < HLen; j++){
            if(j % psCount == 0){
                pa[paIdx] = morphism[j];
                paIdx++;
                paLen++;
            } else if(j % psCount == 1){
                pb[pbIdx] = morphism[j];
                pbIdx++;
                pbLen++;
            } else if(j % psCount == 2){
                pc[pcIdx] = morphism[j];
                pcIdx++;
                pcLen++;
            } else if(j % psCount == 3){
                sa[saIdx] = morphism[j];
                saIdx++;
                saLen++;
            } else if(j % psCount == 4){
                sb[sbIdx] = morphism[j];
                sbIdx++;
                sbLen++;
            } else if(j % psCount == 5){
                sc[scIdx] = morphism[j];
                scIdx++;
                scLen++;
            } else {
                printf("ERROR: Did you change psCount without changing the loop that constructs the suffixes and predfixes?\n");
            }
        }
        reverse(sa, saLen);
        reverse(sb, sbLen);
        reverse(sc, scLen);

        // printf("before concat pa: ");
        // printIntArray(pa, paLen, 0);
        // printf("before concat pb: ");
        // printIntArray(pb, pbLen, 0);
        // printf("before concat pc: ");
        // printIntArray(pc, pcLen, 0);
        // printf("before concat sa: ");
        // printIntArray(sa, saLen, 0);
        // printf("before concat sb: ");
        // printIntArray(sb, sbLen, 0);
        // printf("before concat sc: ");
        // printIntArray(sc, scLen, 0);

        // vtm is squarefree
        concat(sa, saLen, pb, pbLen, ab);
        concat(sa, saLen, pc, pcLen, ac);
        concat(sb, sbLen, pa, paLen, ba);
        concat(sb, sbLen, pc, pcLen, bc);
        concat(sc, scLen, pa, paLen, ca);
        concat(sc, scLen, pb, pbLen, cb);

        // printf("ab: ");
        // printIntArray(ab, saLen + pbLen, 0);
        // printf("ac: ");
        // printIntArray(ac, saLen + pcLen, 0);
        // printf("ba: ");
        // printIntArray(ba, sbLen + paLen, 0);
        // printf("bc: ");
        // printIntArray(bc, sbLen + pcLen, 0);
        // printf("ca: ");
        // printIntArray(ca, scLen + paLen, 0);
        // printf("cb: ");
        // printIntArray(cb, scLen + pbLen, 0);

        if(avoid_yxyprimex(ab, saLen + pbLen, yLen, xLen) &&
        n_p_powerfree(ab, saLen + pbLen, n, p, plus) &&
        avoid_yxyprimex(ac, saLen + pcLen, yLen, xLen) &&
        n_p_powerfree(ac, saLen + pcLen, n, p, plus) &&
        avoid_yxyprimex(ba, sbLen + paLen, yLen, xLen) &&
        n_p_powerfree(ba, sbLen + paLen, n, p, plus) &&
        avoid_yxyprimex(bc, sbLen + pcLen, yLen, xLen) &&
        n_p_powerfree(bc, sbLen + pcLen, n, p, plus) &&
        avoid_yxyprimex(ca, scLen + paLen, yLen, xLen) &&
        n_p_powerfree(ca, scLen + paLen, n, p, plus) &&
        avoid_yxyprimex(cb, scLen + pbLen, yLen, xLen) &&
        n_p_powerfree(cb, scLen + pbLen, n, p, plus)){
            backtrack = 0;
            extend = 1;

            // Walnut can only deal with uniform morphisms
            if(HLen % 3 == 0 && (HLen / 3) % 2 == 1){
                int hMaxLen = paLen + saLen;
                int h0[hMaxLen]; 
                int h1[hMaxLen]; 
                int h2[hMaxLen]; 
                concat(pa, paLen, sa, saLen, h0);
                concat(pb, pbLen, sb, sbLen, h1);
                concat(pc, pcLen, sc, scLen, h2);
                
                int h1Len = (hMaxLen + 1) / 2;
                if (h1Len != ((HLen / 3) + 1) / 2) printf("line 138, hMaxLen HLen issues\n");
                for (int h0Len = hMaxLen; h0Len > 0; h0Len--){
                    int h2Len = hMaxLen + 1 - h0Len;
                    int postMorphMaxLen = hMaxLen * preLen;
                    int postMorph[postMorphMaxLen];
                    int postMorphLen = apply_tern_morph(pre, preLen, h0, h0Len, h1, h1Len, h2, h2Len, postMorph);

                    if(avoid_yxyprimex(postMorph, postMorphLen, yLen, xLen) &&
                    n_p_powerfree(postMorph, postMorphLen, n, p, plus)){

                        fp = fopen("nonuniVTMyxyprimex.txt", "a");   // could add checks for error opening file
                        fprintf(fp, "0->");
                        filePrintIntArray(fp, h0, h0Len, 0);
                        fprintf(fp, "1->");
                        filePrintIntArray(fp, h1, h1Len, 0);
                        fprintf(fp, "2->");
                        filePrintIntArray(fp, h2, h2Len, 0);
                        fclose(fp);

                        return 1;
                    }
                } 
                if(i == maxMLen - 1){
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
            fp = fopen("nonuniVTMyxyprimex.txt", "a");   // could add checks for error opening file
            fprintf(fp, "checked %d potential sequences in %ld seconds\n", count, now - start);
            fclose(fp);
        }
    }
    printf("ERROR: i should never >= maxMLen, as i only advance in if(extend)\n");
    return -1;
}

int main(){
    static int vtmLen = 20;
    int vtm[vtmLen];
    int h0[] = {1};
    int h1[] = {2, 0};
    int h2[] = {2, 1, 0};
    ternary_seq_build(2, vtm, vtmLen, h0, 1, h1, 2, h2, 3);
    // printIntArray(vtm, vtmLen, 0);

    int yLen = 2;
    int xLen = 2;
    int n = 5;
    int p = 2;
    int plus = 1;
    int ltrMLen = 55;
    int res = backtrack_search(vtm, vtmLen, yLen, xLen, n, p, plus, ltrMLen);

    while(res == 0){
        ltrMLen += 5;
        res = backtrack_search(vtm, vtmLen, yLen, xLen, n, p, plus, ltrMLen);
    }
    // printf("backtrack search found result? %d\n", res);

}

