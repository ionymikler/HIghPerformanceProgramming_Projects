#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// void addMatrices(int **matrixA, int **matrixB, int **resultMatrix){
//     for (int i = 0; i < ROWSA;i++){
//         for(int j =0;j<COLSA;j++){
//             resultMatrix[i][j] = matrixA[i][j] + matrixB[i][j];
//         };
//     };
// };

void multiplicationMatrix(int N, int M, int K, int **matrixA, int **matrixB, int** resultMatrix){
    for (int i =0; i < M; i++){
        for (int j = 0; j < N; j++){
            resultMatrix[i][j] = 0;
            for (int k =0; k < K; k++){
                resultMatrix[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
}

void RandomMatrix(int **matrix, int rows, int cols){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols;j++){
            matrix[i][j] = rand() % 2;
            //printf("i: %d, j: %d\n", i, j);
        }
    }
}

void printMatrix(int **matrix,int rows, int cols){
    for(int i = 0; i < rows; i++){
        for(int j = 0;j < cols; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    };
};

int main(){
    int m = 3;
    int n = 3;
    int k = 4;

    int **matrixA = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++) {
        matrixA[i] = (int *)malloc(k * sizeof(int));
    }

    int **matrixB = (int **)malloc(k * sizeof(int *));
    for (int i = 0; i < k; i++) {
        matrixB[i] = (int *)malloc(n * sizeof(int));
    }
    int **resultMatrix = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++) {
        resultMatrix[i] = (int *)malloc(n * sizeof(int));
    };
    int n_rounds = 1;
    clock_t start, end;
    double cpu_time_used;
    
    RandomMatrix(matrixA,m,k);
    RandomMatrix(matrixB,k,n);

    start = clock();
    for(int i = 0;i < n_rounds;i++){
        multiplicationMatrix(n,m,k,matrixA,matrixB,resultMatrix);      
    }
    printMatrix(resultMatrix, m,n);  
    printMatrix(matrixA,m,k);
    printMatrix(matrixB,k,n);
    end = clock();
    free(matrixA);
    free(matrixB);
    // printMatrix(resultMatrix);
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%f", cpu_time_used);
    return 0;
}