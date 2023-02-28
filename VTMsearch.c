// gcc -O3 -o powerfree powerfree.c

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

// round up a divided by b
int ceiling(int a, int b){ return (a+b-1)/b; }

void printIntArray(int arr[], int arrSize, int space){
    for(int i = 0; i < arrSize; i++){
        if(space) printf("%d  ", arr[i]);
        else printf("%d", arr[i]);
    }
    printf("\n");
}

void filePrintIntArray(FILE *fp, int arr[], int arrSize, int space){
    for(int i = 0; i < arrSize; i++){
        if(space) fprintf(fp, "%d  ", arr[i]);
        else fprintf(fp, "%d", arr[i]);
    }
    fprintf(fp,"\n");
}

int fixed_n_p_powerfree(int str[], int sLen, int n, int p){
    // for each letter in str
    for(int i = 0; i <= sLen - n; i++){
        int halt = 0;
        // for each letter in period
        for(int j = 0; j < p; j++){
            for(int step = p; j + step < n; step += p){
                if(str[i + j] != str[i + j + step]){
                    halt = 1;
                    break;
                }
            }
            if(halt) break;
        }
        if(!halt){
        // printf("%d/%d power found at index %d: ", n, p, i);
        // printIntArray(str, sLen, 0);
            return 0;
        }
    }
    return 1;
}

// finite length string, n is length of factor, p is length of period
int n_p_powerfree(int str[], int sLen, int n, int p, int plus){
    // x is each period length
    // n/p = the exponent, ceil((x * n + c) / p), c is 1 if plus
    for(int x = 1; ceiling(x * n + plus, p) <= sLen; x++){
        if(!fixed_n_p_powerfree(str, sLen, ceiling(x * n + plus, p), x)) return 0;
    }
    return 1;
}

// // assume h0 and h1 are the same length
// void apply_bin_morph(int pre[], int preLen, int h0[], int h1[], int h0Len, int res[preLen * h0Len]){
//     for(int i = 0; i < preLen; i++){
//         for(int j = 0; j < h0Len; j++){
//             res[i * h0Len + j] = (pre[i] == 0)? h0[j] : h1[j];
//         }
//     }
// }

// no restriction on whether h0, h1, h2 lengths are the same
// make sure res is large enough
int apply_tern_morph(int pre[], int preLen, int h0[], int h0Len, int h1[], int h1Len, int h2[], int h2Len, int res[]){
    int resIdx = 0;
    for(int i = 0; i < preLen; i++){
        if(pre[i] == 0){
            for(int j = 0; j < h0Len; j++){
                res[resIdx] = h0[j];
                resIdx++;
            }
        } else if(pre[i] == 1){
            for(int j = 0; j < h1Len; j++){
                res[resIdx] = h1[j];
                resIdx++;
            } 
        } else {
            for(int j = 0; j < h2Len; j++){
                res[resIdx] = h2[j];
                resIdx++;
            }
        }
    }
    return resIdx;
}

int fixed_len_avoid_yxyprimex(int str[], int sLen, int yLen, int xLen){
    for(int i = 0; i <= sLen - (yLen + xLen) * 2; i++){
        int halt = 0;
        // test the y and y' in y x y' x
        for(int yi = 0; yi < yLen; yi++){
            if(str[i + yi] == str[i + yLen + xLen + yi]) {halt = 1; break;}
        } 
        // test the x and x in y x y' x
        if(!halt){
            for(int xi = 0; xi < xLen; xi++){
                if(str[i + yLen + xi] != str[i + yLen + xLen + yLen + xi]) {halt = 1; break;}
            }
        }
        if(!halt){
        // printf("yxy\'x, ylen = %d, xlen = %d, found at index %d: ", yLen, xLen, i);
        // printIntArray(str, sLen, 0);
            return 0;
        }
    }
    return 1;
}

// str is binary, xLen and yLen are the min length
int avoid_yxyprimex(int str[], int sLen, int yLen, int xLen){
    for(int i = yLen; (i + xLen) * 2 <= sLen; i++){
        for(int j = xLen; (i + j) * 2 <= sLen; j++){
            if(!fixed_len_avoid_yxyprimex(str, sLen, i, j)) return 0;
        }
    }
    return 1;
}

// concatenate s2 to the end of s1, result is s1s2
void concat(int s1[], int s1Len, int s2[], int s2Len, int s1s2[s1Len + s2Len]){
    for(int i = 0; i < s1Len; i++) s1s2[i] = s1[i];
    for(int i = 0; i < s2Len; i++) s1s2[i + s1Len] = s2[i];    
}

void reverse(int str[], int sLen){
    int tmp[sLen];
    for(int i = 0; i < sLen; i++){
        tmp[sLen - 1 - i] = str[i];
    }
    for(int i = 0; i < sLen; i++){
        str[i] = tmp[i];
    }
}

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
    fp = fopen("yxyprimexVTM.txt", "a");   // could add checks for error opening file
    fprintf(fp, "ltrMLen: %d\n", ltrMLen);
    fclose(fp);

    while(i < maxMLen){
        // printf("************************\n");
        // printf("morphism: ");
        // printIntArray(morphism, i + 1, 0);
        int extend = 0;
        int backtrack = 1;

        int hLen = i + 1;                       // the morphism length so far
        int paIdx = 0, pbIdx = 0, pcIdx = 0, saIdx = 0, sbIdx = 0, scIdx = 0;
        int paLen = 0, pbLen = 0, pcLen = 0, saLen = 0, sbLen = 0, scLen = 0;       // need the precise length for use later

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
            if(hLen % 3 == 0){
                int h0Len = paLen + saLen;
                int h0[h0Len]; 
                int h1[h0Len]; 
                int h2[h0Len]; 
                concat(pa, paLen, sa, saLen, h0);
                concat(pb, pbLen, sb, sbLen, h1);
                concat(pc, pcLen, sc, scLen, h2);

                int postMorphLen = h0Len * preLen;
                int postMorph[postMorphLen];
                apply_tern_morph(pre, preLen, h0, h0Len, h1, h0Len, h2, h0Len, postMorph);

                if(avoid_yxyprimex(postMorph, postMorphLen, yLen, xLen) &&
                n_p_powerfree(postMorph, postMorphLen, n, p, plus)){

                    fp = fopen("yxyprimexVTM.txt", "a");   // could add checks for error opening file
                    fprintf(fp, "0->");
                    filePrintIntArray(fp, h0, h0Len, 0);
                    fprintf(fp, "1->");
                    filePrintIntArray(fp, h1, h0Len, 0);
                    fprintf(fp, "2->");
                    filePrintIntArray(fp, h2, h0Len, 0);
                    fclose(fp);
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
        if(count % 1000000 == 0){
            now = time(NULL);
            fp = fopen("yxyprimexVTM.txt", "a");   // could add checks for error opening file
            fprintf(fp, "checked %d potential sequences in %ld seconds\n", count, now - start);
            fclose(fp);
        }
    }
    printf("ERROR: i should never >= maxMLen, as i only advance in if(extend)\n");
    return -1;
}

void vtm_build(int vtm[], int vtmLen){
    vtm[0] = 2;
    int source = 0;
    int p = 0;
    while(p < vtmLen){
        if(vtm[source] == 0){
            vtm[p] = 1;
            p++;
        } else if(vtm[source] == 1){
            vtm[p] = 2;
            p++; if(p == vtmLen) break;
            vtm[p] = 0;
            p++;
        } else {
            vtm[p] = 2;
            p++; if(p == vtmLen) break;
            vtm[p] = 1;
            p++; if(p == vtmLen) break;
            vtm[p] = 0;
            p++;
        }
        source++;
    }
}

int main(){
    static int vtmLen = 20;
    int vtm[vtmLen];
    vtm_build(vtm, vtmLen);

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






    // FILE *fp;
    // fp = fopen("yxyprimexVTM.txt", "a");   // could add checks for error opening file
    // fprintf(fp, "ltrMLen: %d\n", ltrMLen);
    // fclose(fp);

    // int n1 = 411;
    // int n2 = 42;
    // int n3 = -4;
    // printf("max of %d, %d, and %d is %d\n", n1, n2, n3, max(n1, n2, n3));

    // int s1[] = {};
    // int s1Len = 0;
    // int s2[] = {1,3,5,6,7};
    // int s2Len = 5;
    // int s1s2[s1Len + s2Len];
    // concat(s1, s1Len, s2, s2Len,s1s2);
    // printIntArray(s1s2, s1Len + s2Len, 1);

    // printf("ceiling of %d/%d: %d\n", 3, 4, ceiling(3, 4));
    // printf("ceiling of %d/%d: %d\n", 2, 1, ceiling(2, 1));

    // printIntArray(vtm, vtmLen, 1);

    // // build TM
    // int tm[300] = {0, 1};
    // for(int x = 2; x < 300; x++) tm[x] = x&1 ? !tm[x-1] : tm[x/2];
    // // printIntArray(tm, 300);

    // int pre[] = {2,1,0,2,0,1};
    // int preLen = 6;
    // int h0[] = {1};
    // int h0Len = 1;
    // int h1[] = {2, 0};
    // int h1Len = 2;
    // int h2[] = {2, 1, 0};
    // int h2Len = 3;
    // int morphed[12];
    // apply_tern_morph(pre, preLen, h0, h0Len, h1, h1Len, h2, h2Len, morphed);
    // printIntArray(morphed, 12);


    // int s[] = {0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    // int sLen = 14;
    // printf("does the string avoid yxy'x? %d\n", fixed_len_avoid_yxyprimex(s, sLen, 3, 3));
    // printf("does the string avoid yxy'x? %d\n", avoid_yxyprimex(s, sLen, 2, 2));
    // avoid_yxyprimex(s, sLen, 2, 2);

    // int pre[] = {0, 1, 1, 0};
    // int preLen = 4; 
    // int h0[] = {3, 4};
    // int h1[] = {2, 6};
    // int h0Len = 2;
    // int morphed[preLen * h0Len];
    // apply_bin_morph(pre, preLen, h0, h1, h0Len, morphed);
    // printIntArray(morphed, preLen * h0Len);

    // int s[] = {2, 1, 2, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3};
    // // int s[] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2};
    // int s[] = {0, 0, 1, 0, 0, 1, 0, 0, 1};
    // int sLen = 9;
    // int n = 3; 
    // int p = 1;
    // printf("is the string %d/%d-powerfree? %d\n", n, p, n_p_powerfree(s, sLen, n, p, 0));
    // printf("is the string %d/%d+-powerfree? %d\n", n, p, n_p_powerfree(s, sLen, n, p, 1));

    // // int s[] = {2, 1, 2, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3};
    // // int s[] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2};
    // int s[] = {0, 0, 1, 0, 0, 1, 0, 0, 1};
    // // int s[] = {0, 0, 1, 1, 1, 0, 0, 0, 1};
    // int sLen = 9;
    // int n = 3; 
    // int p = 1;
    // // given 001001001 and 3/1 power, fixed..() should give 1
    // // given 001110001 and 3/1 power, fixed..() should give 0
    // // printf("is the string %d/%d-powerfree? %d\n", n, p, fixed_n_p_powerfree(s, sLen, n, p));
    // printf("is the string %d/%d-powerfree? %d\n", n, p, n_p_powerfree(s, sLen, n, p, 1));
}

