// use the makefile, need fns in the header

#include<stdio.h>
#include<stdlib.h>
// #include<time.h>
#include "powerfree.h"

int backtrack_search(int maxLen, int yLen, int xLen){
    int searchStr[maxLen];
    int i = 0; 
    searchStr[i] = 0;

    int backtrack;
    while(i < maxLen){
        int sLen = i + 1;
        backtrack = 1;

        if(avoid_yxyprimex(searchStr, sLen, yLen, xLen)){
            printf("len %d: \'", sLen);
            printIntArray(searchStr, sLen, 0);
            if(i < maxLen - 1){
                backtrack = 0;
                i++; 
                searchStr[i] = 0;
            }
        } 
        if(backtrack){
            // printf("BACKTRACK\n");
            if(searchStr[i] == 0) searchStr[i] = 1;
            else { 
                while(searchStr[i] == 1){
                    i--; 
                    if(i == 0) return 0;
                }
                searchStr[i] = 1;
            }
        }
    }
    printf("ERROR: i should never >= maxMLen, as i only advance in if(extend)\n");
    return -1;
}

int main(){
    int yLen = 0;
    int xLen = 2;
    backtrack_search(10, yLen, xLen);


    // int n = 5;
    // int p = 2;
    // int plus = 1;


}

