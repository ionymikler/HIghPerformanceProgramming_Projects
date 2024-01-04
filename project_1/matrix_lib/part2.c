#include <stdio.h>
#include "part2.h"

void multiplicationMatrix(int N, int M, int K, int **matrixA, int **matrixB, int **resultMatrix)
{
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            resultMatrix[i][j] = 0;
            for (int k = 0; k < K; k++)
            {
                resultMatrix[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
}

void matmult_mnk(int m,int n,int k,double **A,double **B,double **C){
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = 0;
            for (int k = 0; k < k; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}
