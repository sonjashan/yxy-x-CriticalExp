// gcc -O3 -o powerfree powerfree.c

#include<stdio.h>
#include<stdlib.h>

// only accurate if n/p is irreducible
// assume finite length
// n is length of factor, p is length of period
// testing n/p+ power is essentially testing for n+1/p
int npPowerfree(int str[], int sLen, int n, int p, int plus){
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


int main(){
    int s[] = {2, 1, 2, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3};
    // int s[] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2};
    int sLen = 16;
    int n = 2; 
    int p = 1;
    printf("is the string %d/%d-powerfree? %d\n", n, p, npPowerfree(s, sLen, n, p, 0));
    printf("is the string %d/%d+-powerfree? %d\n", n, p, npPowerfree(s, sLen, n, p, 1));
}


// printf("i = %d\n", i);
// printf("j = %d\n", j);
// printf("step = %d\n", step);
