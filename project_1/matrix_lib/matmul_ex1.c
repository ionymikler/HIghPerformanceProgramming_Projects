#include <stdio.h>
#include <cblas.h>
#include <stdlib.h>
#include "matmul_ex1.h"

//Matrix multiplication function (all matrixes are generated with RandomMatrix)
//MatrixA is[m, k]
//MatrixB is[k, n]
//MatrixC is[m, n]
void matult_lib(int n, int m, int k, double **A, double **B, double **C){
    cblas_dgemm(CblasRowMajor,
                CblasNoTrans,
                CblasNoTrans,
                m, n, k,
                1.0, A[0], k,
                B[0], n, 0.0,
                C[0], n);
}

void matmult_nat(int n, int m, int k, double **A, double **B, double **C)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            C[i][j] = 0;
            for (int _k = 0; _k < k; _k++)
            {
                C[i][j] += A[i][_k] * B[_k][j];
            }
        }
    }
}

// Random matrix 
void RandomMatrix(double **matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] = rand() % 2;
            // printf("i: %d, j: %d\n", i, j);
        }
    }
}

void printMatrix(double **matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%f ", matrix[i][j]);
        }
        printf("\n");
    };
};
