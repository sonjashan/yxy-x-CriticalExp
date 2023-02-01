// gcc -O3 -o powerfree powerfree.c

#include<stdio.h>
#include<stdlib.h>

// only accurate if n/p is irreducible
// assume finite length
// n is length of factor, p is length of period
// testing n/p+ power is essentially testing for n+1/p
int n_p_power_free(int str[], int sLen, int n, int p, int plus){
    while(n <= sLen){
        // for each letter in str
        for(int i = 0; i <= sLen - (n + plus); i++){
            int halt = 0;
            // for each letter in period
            for(int j=0; j < p; j++){
                for(int step = p; j + step < (n + plus); step += p){
                    // printf("n = %d\n", n);
                    // printf("p = %d\n", p);
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
        n += n;
        p += p;
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
void apply_tern_morph(int pre[], int preLen, int h0[], int h0Len, int h1[], int h1Len, int h2[], int h2Len, int res[]){
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
}

void printIntArray(int arr[], int arrSize, int space){
    for(int i = 0; i < arrSize; i++){
        if(space) printf("%d  ", arr[i]);
        else printf("%d", arr[i]);
    }
    printf("\n");
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
// minimum |y| and |x|
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

// this is DFS!
// for pre morphism sequences from 3 letter alphabets like vtm
// xLen and yLen are the min length
// mLen is the max length of morphisms we are looking for
int backtrack_search(int pre[], int preLen, int yLen, int xLen, int n, int p, int plus, int ltrMLen){
    int mLen = ltrMLen * 3;
    int morphism[mLen];
    int i = 0; 
    morphism[i] = 0;
    while(i < mLen){
        if((i + 1) % 3 == 0){
            int h1start = (i + 1) / 3;         // this is also the len of morphism for each letter
            int h2start = (i + 1) / 3 * 2;
            if(avoid_yxyprimex(morphism, h1start, yLen, xLen) && 
            n_p_power_free(morphism, h1start, n, p, plus) &&

            avoid_yxyprimex(morphism + h1start, h1start, yLen, xLen) &&
            n_p_power_free(morphism + h1start, h1start, n, p, plus) &&

            avoid_yxyprimex(morphism + h2start, h1start, yLen, xLen) &&
            n_p_power_free(morphism + h2start, h1start, n, p, plus)){
                int postMorphLen = preLen * h1start;
                int postMorph[postMorphLen];
                apply_tern_morph(pre, preLen, morphism, h1start, morphism + h1start, h1start, morphism + h2start, h1start, postMorph);
                
                if(avoid_yxyprimex(postMorph, postMorphLen, yLen, xLen) &&
                n_p_power_free(postMorph, postMorphLen, n, p, plus)){

                    printf("0->");
                    printIntArray(morphism, h1start, 0);
                    printf("1->");
                    printIntArray(morphism + h1start, h1start, 0);
                    printf("2->");
                    printIntArray(morphism + h2start, h1start, 0);
                    return 1;
                }
            }
        } else {
            i++; 
            if (i < mLen) morphism[i] = 0;
            continue;
        }
        if(morphism[i] == 0) morphism[i] = 1;
        else { 
            while(morphism[i] == 1){
                i--; 
                if(i < 0) return -1;
            }
            morphism[i] = 1;
        }
    }
}

void vtmBuild(int vtm[], int vtmLen){
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
    vtmBuild(vtm, vtmLen);

    int yLen = 2;
    int xLen = 2;
    int n = 2;
    int p = 1;
    int plus = 0;
    int ltrMLen = 60;

    backtrack_search(vtm, vtmLen, yLen, xLen, n, p, plus, ltrMLen);


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
    // int sLen = 16;
    // int n = 2; 
    // int p = 1;
    // printf("is the string %d/%d-powerfree? %d\n", n, p, n_p_power_free(s, sLen, n, p, 0));
    // printf("is the string %d/%d+-powerfree? %d\n", n, p, n_p_power_free(s, sLen, n, p, 1));
}

