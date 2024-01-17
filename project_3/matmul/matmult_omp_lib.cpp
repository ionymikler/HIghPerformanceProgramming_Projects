extern "C"{
#include <stdio.h>
#include <stdlib.h>
#include <cblas.h>
#include <omp.h>
#include "matmult_omp_lib.h"
#define MIN(a,b) \
({ __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a <= _b ? _a : _b; })

int NUM_TEAMS = 114, THREADS_PER_TEAM = 32; //QUESTION: How mamy threads per team

void init_C_dev(int m, int n, double **C, int num_teams, int threads_per_team){
    // Remember; array mapping is [lower:length], NOT [lower:upper]
    #pragma omp target teams distribute parallel for \
        map(to:m,n), map(from:C[0:m][0:n]) \
        num_teams(num_teams) thread_limit(threads_per_team) // QUESTION: Should we hard code these values?
        // collapse(2)
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++){
            C[i][j] = 0.0;
        }
    }

}

void init_C(int m,int n,double **C){
    #pragma omp parallel for collapse(2) \
        shared(C, m,n )
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            #pragma omp critical
            {
                C[i][j] = 0.0;
            }
        }
    }
}

double get_sum_u(double **u, int N){
    double sum = 0;
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
                sum += u[i][j];
        }
    }
    return sum;
}

void matmult_mkn_omp(int m,int n,int k,double **A,double **B,double **C){
    init_C(m,n,C);

    
    #pragma omp parallel for collapse(2) \
        shared(A,B,C, m,n,k)
        for (int row=0; row <m; row++){
            for (int q=0; q<k; q++){
                for (int col=0; col < n; col++){
                    C[row][col] += A[row][q] * B[q][col];
                }
            }
        }
}

void matmult_mkn_offload(int m,int n,int k,double **A,double **B,double **C){
    init_C_dev(m,n,C, NUM_TEAMS, THREADS_PER_TEAM);
    
    #pragma omp target teams distribute parallel for \
        map(to:m,n,k,A[0:m][0:k],B[0:k][0:n]), map(from:C[0:m][0:n]) 
        for (int i = 0; i < m; i++){
            for (int q = 0; q < k; q++){
                for (int j = 0; j < n; j++){
                    C[i][j] += A[i][q] * B[q][j];
                }
            }
        } // END PARALLEL FOR
}

void matmult_mnk_offload(int m,int n,int k,double **A,double **B,double **C){
    init_C_dev(m,n,C, NUM_TEAMS, THREADS_PER_TEAM);

    #pragma omp target teams distribute parallel for \
        map(to:m,n,k,A[0:m][0:k],B[0:k][0:n]), map(from:C[0:m][0:n]) collapse(2)
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            double sum = 0.0;
            for (int q = 0; q < k; q++){
                sum += A[i][q] * B[q][j];
            }
            C[i][j] = sum;
        }
    } // END PARALLEL FOR
}

void matmult_blk_offload(int m,int n,int k,double **A,double **B,double **C){
    init_C_dev(m,n,C, NUM_TEAMS, THREADS_PER_TEAM);

    #define BLK 50

    #pragma omp target teams distribute parallel for collapse(2) \
        map(to:m,n,k,A[0:m][0:k],B[0:k][0:n]), map(from:C[0:m][0:n])
    for (int i = 0; i < m; i+=BLK){
        for (int j = 0; j < n; j++){
            if (i + BLK - 1 < m){
                double blk_items[BLK] = {0}; // All initialized to 0
                for (int ii=0; ii<BLK;ii++){ // Calculate elements block [i,i+blk)
                    blk_items[ii] = 0;
                    for (int q=0; q<k; q++){ // Single element in block
                        blk_items[ii] += A[i+ii][q] * B[q][j];
                    }
                }
                for (int sii=0;sii<BLK;sii++){
                    C[i+sii][j] = blk_items[sii];
                }
            }else{ // elements in the last block (which is smaller)
                double blk_items[BLK-m]; // All initialized to 0
                for (int ii=0;ii=(BLK-m);ii++){
                    for (int q=0; q<k; q++){
                        blk_items[ii] = A[i+ii][q] * B[q][j];
                    }
                }
                for (int sii=0;sii<(BLK-m);sii++){
                    C[i+sii][j] = blk_items[sii];
                }
            }
        }
    }
}

void matmult_lib(int m, int n, int k, double **A, double **B, double **C){
    cblas_dgemm(CblasRowMajor,
                CblasNoTrans,
                CblasNoTrans,
                m, n, k,
                1.0, A[0], k,
                B[0], n, 0.0,
                C[0], n);
}
}