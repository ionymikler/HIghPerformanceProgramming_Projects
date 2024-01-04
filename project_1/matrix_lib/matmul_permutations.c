#include <stdio.h>
#include "matmul_permutations.h"



void matmult_mnk(int m,int n,int k,double **A,double **B,double **C){
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = 0;
            for (int q = 0; q < k; q++)
            {
                C[i][j] += A[i][q] * B[q][j];
            }
        }
    }
} 

void matmult_nmk(int m,int n,int k,double **A,double **B,double **C){
    for (int col=0; col < n; col++){
        for (int row=0; row <m; row++){
            C[row][col]=0;
            for (int q=0; q<k; q++){
                C[row][col] += A[row][q] * B[q][col];
            }
        }
    }
} 