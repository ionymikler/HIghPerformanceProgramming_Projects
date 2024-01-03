#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//Matrix multiplication function (all matrixes are generated with RandomMatrix)
//MatrixA is[m, k]
//MatrixB is[k, n]
//MatrixC is[m, n]
    void
    multiplicationMatrix(int N, int M, int K, int **matrixA, int **matrixB, int **resultMatrix)
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
// Random matrix 
void RandomMatrix(int **matrix, int rows, int cols)
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

void printMatrix(int **matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    };
};

int main()
{
    // Adjust for matrix sizes:
    //MatrixA is[m, k]
    //MatrixB is[k, n]
    int m = 3;
    int n = 3;
    int k = 4;

    //n_rounds number of times matrix multiplication will be executed 
    int n_rounds = 1;
    clock_t start, end;
    double cpu_time_used;

    // Initialize matrix A
    int **matrixA = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++)
    {
        matrixA[i] = (int *)malloc(k * sizeof(int));
    }
    //Initialize matrix B
    int **matrixB = (int **)malloc(k * sizeof(int *));
    for (int i = 0; i < k; i++)
    {
        matrixB[i] = (int *)malloc(n * sizeof(int));
    }
    //Initialize Matrix C
    int **resultMatrix = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++)
    {
        resultMatrix[i] = (int *)malloc(n * sizeof(int));
    };
    
    // Populize Matrix A and B
    RandomMatrix(matrixA, m, k);
    RandomMatrix(matrixB, k, n);
    
    // Perform matrix multiplication
    start = clock();
    for (int i = 0; i < n_rounds; i++)
    {
        multiplicationMatrix(n, m, k, matrixA, matrixB, resultMatrix);
    }

    //Print the results for 
    printMatrix(resultMatrix, m, n);
    printMatrix(matrixA, m, k);
    printMatrix(matrixB, k, n);
    end = clock();
    
    //Clean the memory
    free(matrixA);
    free(matrixB);
    free(resultMatrix);

    
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%f", cpu_time_used);
    return 0;
}