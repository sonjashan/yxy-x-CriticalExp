// gcc -O3 -o powerfree powerfree.c
// use the makefile

#include<stdio.h>
#include<stdlib.h>
#include "powerfree.h"

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

// assume h0 and h1 are the same length
void apply_bin_morph(int pre[], int preLen, int h0[], int h1[], int h0Len, int res[preLen * h0Len]){
    for(int i = 0; i < preLen; i++){
        for(int j = 0; j < h0Len; j++){
            res[i * h0Len + j] = (pre[i] == 0)? h0[j] : h1[j];
        }
    }
}

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

// int main(){

    // FILE *fp;
    // fp = fopen("yxyprimexVTM.txt", "a");   // could add checks for error opening file
    // fprintf(fp, "ltrMLen: %d\n", ltrMLen);
    // fclose(fp);

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
// }

