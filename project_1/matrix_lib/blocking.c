#define N 1000 // Size of the matrices
#define BLOCK_SIZE 50 // Block size for blocking
 #define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a <= _b ? _a : _b; })

     
void matmult_blk(int m,int n,int k,double **A,double **B,double **C, int bs){
    #include <stdio.h>

    int i, j, _k, ii, jj, kk;
    for (i = 0; i <m; i++){
        for (j=0;j<n;j++){
            C[i][j] = 0.0;
        }
    }
    for (i = 0; i < m; i += bs) {
        for (j = 0; j < n; j += bs) {
            for (_k = 0; _k < k; _k += bs) {

                // Multiply blocks
                for (ii = 0; ii < MIN(m-i,bs); ii++) {
                    for (jj = 0; jj < MIN(n-j,bs); jj++) {
                        for (kk = 0; kk < MIN(k-_k, bs); kk++) {
                            C[i+ii][j+jj] += A[i+ii][_k+kk] * B[_k+kk][j+jj];
                        }
                    }
                }
            }
        }
    }
}