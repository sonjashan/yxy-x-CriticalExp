// gcc -O3 -o powerfree powerfree.c

#include<stdio.h>
#include<stdlib.h>

// to do np+ powerfree
// if n%p == 0

// assume finite length
// n is length of factor, p is length of period
int npPowerfree(int str[], int sLen, int n, int p){
    // for each letter in str
    for(int i=0; i <= sLen - n; i++){
        int halt = 0;
        // for each letter in period
        for(int j=0; j < p; j++){
            for(int step = p; j + step < n; step += p){
                if(str[i + j] != str[i + j + step]){
                    halt = 1;
                    break;
                }
            }
            if(halt) break;
        }
        if(!halt) return 0;
    }
    return 1;
}


int main(){
    int s[] = {2, 1, 2, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3};
    int sLen = 16;
    int n = 13; 
    int p = 6;
    printf("is the string %d/%d-powerfree? %d\n", n, p, npPowerfree(s, sLen, n, p));
}


// printf("i = %d\n", i);
// printf("j = %d\n", j);
// printf("step = %d\n", step);
