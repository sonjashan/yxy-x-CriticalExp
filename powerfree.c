// gcc -O3 -o powerfree powerfree.c

#include<stdio.h>
#include<stdlib.h>

void printIntArray(int arr[], int arrSize, int space){
    for(int i = 0; i < arrSize; i++){
        if(space) printf("%d  ", arr[i]);
        else printf("%d", arr[i]);
    }
    printf("\n");
}

// only accurate if n/p is irreducible
// assume finite length
// n is length of factor, p is length of period
// testing n/p+ power is essentially testing for n+1/p
int n_p_powerfree(int str[], int sLen, int n, int p, int plus){
    // like n * k where k is 1, 2, ... 
    int nK = n;
    int pK = p;
    while(nK <= sLen){
        // for each letter in str
        for(int i = 0; i <= sLen - (nK + plus); i++){
            int halt = 0;
            // for each letter in period
            for(int j=0; j < pK; j++){
                for(int step = pK; j + step < (nK + plus); step += pK){
                    // printf("n = %d\n", nK);
                    // printf("p = %d\n", pK);
                    // printf("i = %d\n", i);
                    // printf("j = %d\n", j);
                    // printf("step = %d\n", step);
                    // printf("--------------\n");

                    if(str[i + j] != str[i + j + step]){
                        halt = 1;
                        break;
                    }
                }
                if(halt) break;
            }
            if(!halt) return 0;
        }
        nK += n;
        pK += p;
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
            // printf("i = %d\n", i);
            return 0;
        }
    }
    return 1;
}

// str is binary
// xLen and yLen are the min length
int avoid_yxyprimex(int str[], int sLen, int yLen, int xLen){
    for(int i = yLen; (i + xLen) * 2 <= sLen; i++){
        for(int j = xLen; (i + j) * 2 <= sLen; j++){
            // printf("i = %d\n", i);
            // printf("j = %d\n", j);
            // printf("--------------\n");
            if(!fixed_len_avoid_yxyprimex(str, sLen, i, j)) return 0;
        }
    }
    return 1;
}

// round up a divided by b
int ceiling(int a, int b){ return (a+b-1)/b; }

// concatenate s2 to the end of s1, result is s1s2
void concat(int s1[], int s1Len, int s2[], int s2Len, int s1s2[s1Len + s2Len]){
    for(int i = 0; i < s1Len; i++) s1s2[i] = s1[i];
    for(int i = 0; i < s2Len; i++) s1s2[i + s1Len] = s2[i];    
}

int max(int n1, int n2, int n3){
    int res = n1;
    if(n2 > res) res = n2;
    if(n3 > res) res = n3;
    return res;
}

// this is DFS!
// for pre morphism sequences from 3 letter alphabets like vtm
// xLen and yLen are the min length as in avoid_yxyprime()
// mLen is the max length of morphisms we are looking for
// return 0 not found, 1 found, -1 error
int backtrack_search(int pre[], int preLen, int yLen, int xLen, int n, int p, int plus, int ltrMLen){
    int psCount = 6;          // vtm uses a 3 letter alphabet and h of a, b, c each has a pre- and suffixes
    int mLen = ltrMLen * 3;
    int morphism[mLen];
    int i = 0; 
    morphism[i] = 0;

    int count = 0;
    while(i < mLen){
        // printIntArray(morphism, i + 1, 0);
        int extend = 0;
        int backtrack = 1;

        int hLen = i + 1;       // the morphism length so far
        int psLen = ceiling(hLen, psCount);     // pre- or suffix max length = ceiling(hLen / psCount)
        int pa[psLen], pb[psLen], pc[psLen], sa[psLen], sb[psLen], sc[psLen];       // pa is prefix of h(a)
        int paIdx, pbIdx, pcIdx = 0;
        int saIdx, sbIdx, scIdx = psLen - 1;
        int paLen, pbLen, pcLen, saLen, sbLen, scLen = 0;       // need the precise length for use later

        // psLen is at least 1/6 of hLen, so should not run out of space
        for(int j = 0; j < hLen; j++){
            if(j % psCount == 0){
                pa[paIdx] = morphism[j];
                paIdx++;
                paLen++;
            } else if(j % psCount == 1){
                sa[saIdx] = morphism[j];
                saIdx--;
                saLen++;
            } else if(j % psCount == 2){
                pb[pbIdx] = morphism[j];
                pbIdx++;
                pbLen++;
            } else if(j % psCount == 3){
                sb[sbIdx] = morphism[j];
                sbIdx--;
                sbLen++;
            } else if(j % psCount == 4){
                pc[pcIdx] = morphism[j];
                pcIdx++;
                pcLen++;
            } else if(j % psCount == 5){
                sc[scIdx] = morphism[j];
                scIdx--;
                scLen++;
            } else {
                printf("ERROR: Did you change psCount without changing the loop that constructs the suffixes and predfixes?\n");
            }
        }

        // vtm is squarefree
        int ab[saLen + pbLen], ac[saLen + pcLen], ba[sbLen + paLen], bc[sbLen + pcLen], ca[scLen + paLen], cb[scLen + pbLen];
        concat(sa, saLen, pb, pbLen, ab);
        concat(sa, saLen, pc, pcLen, ac);
        concat(sb, sbLen, pa, paLen, ba);
        concat(sb, sbLen, pc, pcLen, bc);
        concat(sc, scLen, pa, paLen, ca);
        concat(sc, scLen, pb, pbLen, cb);

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

            int h0Len = paLen + saLen;
            int h0[h0Len]; 
            concat(pa, paLen, sa, saLen, h0);
            int h1Len = pbLen + sbLen;
            int h1[h1Len]; 
            concat(pb, pbLen, sb, sbLen, h1);
            int h2Len = pcLen + scLen;
            int h2[h2Len]; 
            concat(pc, pcLen, sc, scLen, h2);

            int postMorph[max(h0Len, h1Len, h2Len) * preLen];
            int postMorphLen = apply_tern_morph(pre, preLen, h0, h0Len, h1, h1Len, h2, h2Len, postMorph);

            if(avoid_yxyprimex(postMorph, postMorphLen, yLen, xLen) &&
            n_p_powerfree(postMorph, postMorphLen, n, p, plus)){

                printf("0->");
                printIntArray(h0, h0Len, 0);
                printf("1->");
                printIntArray(h1, h1Len, 0);
                printf("2->");
                printIntArray(h2, h2Len, 0);
                return 1;

            } else if(i >= mLen - 1){
                extend = 0;
                backtrack = 1;
            }
        }        
        if(extend){
            // check for i above 
            // if we are here, i < mLen - 1
            i++; 
            morphism[i] = 0;
        } 
        if(backtrack){
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
        if(count % 10000 == 0) printf("checked %d potential sequences\n", count);
    }
    printf("ERROR: i should never >= mLen, as i only advance in if(extend)\n");
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
    // static int vtmLen = 20;
    // int vtm[vtmLen];
    // vtm_build(vtm, vtmLen);

    // int yLen = 2;
    // int xLen = 2;
    // int n = 5;
    // int p = 2;
    // int plus = 1;
    // int ltrMLen = 30;

    // int res = backtrack_search(vtm, vtmLen, yLen, xLen, n, p, plus, ltrMLen);
    // printf("backtrack search found result? %d\n", res);





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
}

