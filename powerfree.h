#ifndef POWERFREE_H
#define POWERFREE_H

/* round up a divided by b (a/b) */
int ceiling(int a, int b);

/* space = 0 if do not want space between array contents */
void printIntArray(int arr[], int arrSize, int space);

/* print array content to file */
void filePrintIntArray(FILE *fp, int arr[], int arrSize, int space);

/* finite length string, whether it avoids n/p power or n/p+ */
int n_p_powerfree(int str[], int sLen, int n, int p, int plus);

/* finite binary string, whether it avoids yxy'x, for |y| and |x| at least yLen and xLen long */
int avoid_yxyprimex(int str[], int sLen, int yLen, int xLen);

/* return h(pre), the length of res is decided by length of pre and h, make sure res is large enough */
int apply_tern_morph(int pre[], int preLen, int h0[], int h0Len, int h1[], int h1Len, int h2[], int h2Len, int res[]);

/* assume h0 and h1 are the same length */
void apply_bin_morph(int pre[], int preLen, int h0[], int h1[], int h0Len, int res[preLen * h0Len]);

/* concatenate s2 to the end of s1, result is s1s2 */
void concat(int s1[], int s1Len, int s2[], int s2Len, int s1s2[s1Len + s2Len]);

/* reverse the string of length sLen */
void reverse(int str[], int sLen);

/* build ternary seq that is the fixed point of the a morphism specified in the parameters */
void ternary_seq_build(int startNum, int res[], int resLen, int h0[], int h0Len, int h1[], int h1Len, int h2[], int h2Len);

/* return 1 if two integer arrays s1 = s2, false otherwise, assume equal length */
int array_equal(int s1[], int s2[], int sLen);

#endif /* POWERFREE_H */
