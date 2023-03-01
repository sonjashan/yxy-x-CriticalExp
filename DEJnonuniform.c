// can only be nonuniform if |h(02)| = 2 * |h(1)|

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "powerfree.h"


// this is DFS
// for pre morphism sequences from 3 letter alphabets like DEJ
// xLen and yLen are the min length as in avoid_yxyprime()
// h1MaxLen is the max |h(1)| of the morphism we are looking for
// return 0 not found, 1 found, -1 error
int backtrack_search(int pre[], int preLen, int yLen, int xLen, int n, int p, int plus, int h1MaxLen){
    // search string: a1 c1 b1 a'1 c'1 a2 c2 b1' a'2 c'2 a3 c3 b2...
    // h(0) = a1...at a't...a'1

    int maxMLen = h1MaxLen * 5 - 2;     // we always stop at after a b# in the search string, 2*(2*h1MaxLen - 1) + h1MaxLen
    int morphism[maxMLen];
    int i = 0; 
    morphism[i] = 0;

    // pre- or suffixes max length is h1MaxLen (out of h1MaxLen-1, ceil(h1MaxLen/2), and h1MaxLen/2)
    int pa[h1MaxLen], pb[h1MaxLen], pc[h1MaxLen], sa[h1MaxLen], sb[h1MaxLen], sc[h1MaxLen];       // pa is prefix of h(a)
    int ab[2*h1MaxLen], ac[2*h1MaxLen], ba[2*h1MaxLen], bc[2*h1MaxLen], ca[2*h1MaxLen], cb[2*h1MaxLen];       // suffix of a concatenated with prefix of b, etc.

    unsigned int count = 0;
    time_t start, now;
    start = time(NULL);
    FILE *fp;
    fp = fopen("nonuniDEJyxyprimex.txt", "a");   // could add checks for error opening file
    fprintf(fp, "h1MaxLen: %d\n", h1MaxLen);
    fclose(fp);

    while(i < maxMLen){
        // printf("************************\n");
        // printf("morphism: ");
        // printIntArray(morphism, i + 1, 0);
        int extend = 0;
        int backtrack = 1;

        int HLen = i + 1;       // the morphism length so far
        int paIdx = 0, pbIdx = 0, pcIdx = 0, saIdx = 0, sbIdx = 0, scIdx = 0;
        int paLen = 0, pbLen = 0, pcLen = 0, saLen = 0, sbLen = 0, scLen = 0;       // need the precise length for use later

        // pa and such is of length h1MaxLen, HLen is at most h1MaxLen * 5 - 2, so should not run out of space
        for(int j = 0; j < HLen; j++){
            if(j % 5 == 0){
                pa[paIdx] = morphism[j];
                paIdx++;
                paLen++;
            } else if(j % 10 == 2){
                pb[pbIdx] = morphism[j];
                pbIdx++;
                pbLen++;
            } else if(j % 5 == 1){
                pc[pcIdx] = morphism[j];
                pcIdx++;
                pcLen++;
            } else if(j % 5 == 3){
                sa[saIdx] = morphism[j];
                saIdx++;
                saLen++;
            } else if(j % 10 == 7){
                sb[sbIdx] = morphism[j];
                sbIdx++;
                sbLen++;
            } else if(j % 5 == 4){
                sc[scIdx] = morphism[j];
                scIdx++;
                scLen++;
            } else {
                printf("ERROR: Did you change the numbers without changing the loop that constructs the suffixes and predfixes?\n");
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

        // DEJ is 7/4+ free
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

        if(n_p_powerfree(ab, saLen + pbLen, n, p, plus) &&
        n_p_powerfree(ac, saLen + pcLen, n, p, plus) &&
        n_p_powerfree(ba, sbLen + paLen, n, p, plus) &&
        n_p_powerfree(bc, sbLen + pcLen, n, p, plus) &&
        n_p_powerfree(ca, scLen + paLen, n, p, plus) &&
        n_p_powerfree(cb, scLen + pbLen, n, p, plus) &&
        avoid_yxyprimex(ab, saLen + pbLen, yLen, xLen) &&
        avoid_yxyprimex(ac, saLen + pcLen, yLen, xLen) &&
        avoid_yxyprimex(ba, sbLen + paLen, yLen, xLen) &&
        avoid_yxyprimex(bc, sbLen + pcLen, yLen, xLen) &&
        avoid_yxyprimex(ca, scLen + paLen, yLen, xLen) &&
        avoid_yxyprimex(cb, scLen + pbLen, yLen, xLen)){
            backtrack = 0;
            extend = 1;

            // non uniform morphisms if |h(02|) = 2 * |h(1)|
            if(HLen % 5 == 3){
                int h1Len = pbLen + sbLen;
                int h0MaxLen = h1Len * 2 - 1;
                // because of the format of the search string
                if (h0MaxLen != paLen + saLen || h0MaxLen != pcLen + scLen) printf("line 138, pre- and suffix length issues\n");
                
                int h0[h0MaxLen];
                int h1[h1Len]; 
                int h2[h0MaxLen]; 
                concat(pa, paLen, sa, saLen, h0);
                concat(pb, pbLen, sb, sbLen, h1);
                concat(pc, pcLen, sc, scLen, h2);
                
                for (int h0Len = h0MaxLen; h0Len > 0; h0Len--){
                    int h2Len = h0MaxLen + 1 - h0Len;

                    // h1Len * 2 = h0Len + h2Len, so if h0Len = h1Len, then h2Len = h1Len
                    // if any two of h0, h1, h2 then we are introducing unnecessary powers
                    if(h1Len == h0Len && (array_equal(h0, h1, h1Len) || array_equal(h1, h2, h1Len) || array_equal(h2, h0, h1Len))) continue;

                    int postMorphMaxLen = h0MaxLen * preLen;
                    int postMorph[postMorphMaxLen];
                    int postMorphLen = apply_tern_morph(pre, preLen, h0, h0Len, h1, h1Len, h2, h2Len, postMorph);

                    // printf("h0: ");
                    // printIntArray(h0, h0Len, 0);
                    // printf("h1: ");
                    // printIntArray(h1, h1Len, 0);
                    // printf("h2: ");
                    // printIntArray(h2, h2Len, 0);
                    // printf("postMorph: ");
                    // printIntArray(postMorph, postMorphLen, 0);

                    if(n_p_powerfree(postMorph, postMorphLen, n, p, plus) && 
                    avoid_yxyprimex(postMorph, postMorphLen, yLen, xLen)){

                        fp = fopen("nonuniDEJyxyprimex.txt", "a");   // could add checks for error opening file
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
        if(count % 100000 == 0){
            now = time(NULL);
            fp = fopen("nonuniDEJyxyprimex.txt", "a");   // could add checks for error opening file
            fprintf(fp, "checked %d potential sequences in %ld seconds\n", count, now - start);
            fclose(fp);
        }
    }
    printf("ERROR: i should never >= maxMLen, as i only advance in if(extend)\n");
    return -1;
}

int main(){
    static int dejLen = 25;
    int dej[dejLen];
    int h0[] = {0, 1, 2, 0, 2, 1, 2, 0, 1, 2, 1, 0, 2, 1, 2, 0, 2, 1, 0};
    int h1[] = {1, 2, 0, 1, 0, 2, 0, 1, 2, 0, 2, 1, 0, 2, 0, 1, 0, 2, 1};
    int h2[] = {2, 0, 1, 2, 1, 0, 1, 2, 0, 1, 0, 2, 1, 0, 1, 2, 1, 0, 2};
    int h0Len = 19;
    int h1Len = 19;
    int h2Len = 19;
    ternary_seq_build(0, dej, dejLen, h0, h0Len, h1, h1Len, h2, h2Len);
    // printIntArray(dej, dejLen, 0);

    int yLen = 2;
    int xLen = 2;
    int n = 5;
    int p = 2;
    int plus = 1;
    int h1MaxLen = 30;
    int res = backtrack_search(dej, dejLen, yLen, xLen, n, p, plus, h1MaxLen);

    while(res == 0){
        h1MaxLen += 5;
        res = backtrack_search(dej, dejLen, yLen, xLen, n, p, plus, h1MaxLen);
    }
    // printf("backtrack search found result? %d\n", res);

}

