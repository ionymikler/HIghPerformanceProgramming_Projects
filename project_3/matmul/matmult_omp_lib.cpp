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

int NUM_TEAMS = 114, THREADS_PER_TEAM = 32; //QUESTION: How man y threads per team

void saveToFile(char myString[100], char filename[100]){
    // printf("Saving to file\n");
    // FILE *fp;
    // fp = fopen(filename, "a");
    // fprintf(fp, "%s", myString);
    // fclose(fp);
}

void init_C_dev(int m, int n, double **C, int num_teams, int threads_per_team){
    // Remember; array mapping is [lower:length], NOT [lower:upper]
    // TODO: Tune TEAMS anf THREADS values
        #pragma omp target teams distribute parallel for \
            num_teams(num_teams) thread_limit(threads_per_team) \
            collapse(2)
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

    double start_t, end_t, dataoff_time, comp_time;
	start_t = omp_get_wtime();
    #pragma omp target data \
        map(to:m,n,k,A[0:m][0:k],B[0:k][0:n]), map(from:C[0:m][0:n])
    {
        init_C_dev(m,n,C, NUM_TEAMS, THREADS_PER_TEAM);
        double comp_time_s= omp_get_wtime();
        #pragma omp target teams distribute parallel for \
            map(to:m,n,k,A[0:m][0:k],B[0:k][0:n])\
            num_teams(NUM_TEAMS) thread_limit(THREADS_PER_TEAM)
            for (int i = 0; i < m; i++){
                for (int q = 0; q < k; q++){
                    for (int j = 0; j < n; j++){
                        C[i][j] += A[i][q] * B[q][j];
                    }
                }
        } // END PARALLEL FOR, END DEVICE AREA
        comp_time= omp_get_wtime() - comp_time_s;
    } // END TARGET DATA

    end_t = omp_get_wtime();
    dataoff_time = (end_t - start_t) - comp_time;

    // Save results to file
    char myString[100]; // Allocate memory for myString array
    sprintf(myString, "%d, %f, %f\n",m, comp_time*1000, dataoff_time*1000);

    char filename[100] = "results/timings/timing_mkn_offload.txt";
    saveToFile(myString, filename);
    // printf("%s", myString);
    
}

void matmult_mnk_offload(int m,int n,int k,double **A,double **B,double **C){

    double start_t, end_t, dataoff_time, comp_time;
	start_t = omp_get_wtime();
    #pragma omp target data \
        map(to:m,n,k,A[0:m][0:k],B[0:k][0:n]), map(from:C[0:m][0:n])
    {
        double comp_time_s= omp_get_wtime();
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
        comp_time= omp_get_wtime() - comp_time_s;
    } // END TARGET DATA

    end_t = omp_get_wtime();
    dataoff_time = (end_t - start_t) - comp_time;
    
    // Save results to file
    char myString[100]; // Allocate memory for myString array
    sprintf(myString, "%d, %f, %f\n",m, comp_time*1000, dataoff_time*1000);
    
    char filename[100] = "results/timings/timing_mnk_offload.txt";
    saveToFile(myString, filename);
    // printf("%s", myString);
}

void matmult_blk_offload(int m,int n,int k,double **A,double **B,double **C){
    #define BLK 4
    double start_time = omp_get_wtime();

    double start_t, end_t, dataoff_time, comp_time;
	start_t = omp_get_wtime();
    #pragma omp target data \
        map(to:m,n,k,A[0:m][0:k],B[0:k][0:n]), map(from:C[0:m][0:n])
    {
        double comp_time_s= omp_get_wtime();
        #pragma omp target teams distribute parallel for collapse(2) \
            map(to:m,n,k,A[0:m][0:k],B[0:k][0:n]), map(from:C[0:m][0:n]) \
            num_teams((m/BLK)) thread_limit(THREADS_PER_TEAM)
        for (int i = 0; i < m; i+=BLK){
            for (int j = 0; j < n; j++){
                if (i + BLK - 1 < m){
                    double blk_items[BLK] = {0};
                    for (int q=0; q<k; q++){
                        for (int ii=0; ii<BLK;ii++){ // Calculate elements block [i,i+blk)
                            blk_items[ii] += A[i+ii][q] * B[q][j];
                        }
                    }
                    for (int sii=0;sii<BLK;sii++){
                        C[i+sii][j] = blk_items[sii];
                    }
                }else{ // elements in the (smaller) last block
                    // version 1
                    for (int ii=0;ii<(m%BLK);ii++){
                        double sum = 0;
                        for (int q=0; q<k; q++){
                            sum += A[i+ii][q] * B[q][j];
                        }
                        C[i+ii][j] = sum;
                    }
                    // version 2
                    // double *sum = (double*) malloc((m%BLK)*sizeof(double));
                    // for (int ii=0;ii<(m%BLK);ii++){
                    //     sum[ii] = 0;
                    // }
                    // for (int q=0; q<k; q++){
                    //     for (int ii=0;ii<(m%BLK);ii++){
                    //         sum[ii] += A[i+ii][q] * B[q][j];
                    //     }
                    // }
                    // for (int ii=0;ii<(m%BLK);ii++){
                    //     C[i+ii][j] = sum[ii];
                    // }
                    // free(sum);
                } // END IF
            }
        }// END PARALLEL FOR
        comp_time= omp_get_wtime() - comp_time_s;
    } // END TARGET DATA

    end_t = omp_get_wtime();
    dataoff_time = (end_t - start_t) - comp_time;

    // Save results to file
    char myString[100]; // Allocate memory for myString array
    sprintf(myString, "%d, %f, %f\n",m, comp_time*1000, dataoff_time*1000);
    
    char filename[100] = "results/timings/timing_blk_offload.txt";
    saveToFile(myString, filename);
    // printf("%s", myString);

}

void matmult_asy_offload(int m,int n,int k,double **A,double **B,double **C){
    #define BLK 4
    #define SPLITS 2
    double start_time = omp_get_wtime();

    double start_t, end_t, dataoff_time, comp_time;
	start_t = omp_get_wtime();

    for (int s=0;s<SPLITS;++s){
        int length = m / SPLITS;
        int start = s * length;
        #pragma omp target enter data nowait map(to:A[start:length][0:k],B[0:k][0:n],C[start:length][0:n])
    }
    #pragma omp taskwait

    // #pragma omp target data \
    //     map(to:m,n,k,A[0:m][0:k],B[0:k][0:n]), map(from:C[0:m][0:n])
    // {
    double comp_time_s= omp_get_wtime();
    for (int s=0;s<SPLITS;++s){
        int length = m / SPLITS;
        int start = s * length;

        #pragma omp target teams distribute parallel for \
        map(to:m,n,k,A[start:length][0:k],B[0:k][0:n]), map(from:C[start:length][0:n]) \
        num_teams(length) thread_limit(THREADS_PER_TEAM) \
        collapse(2)            
        for (int i = 0; i < m; i+=BLK){
            for (int j = 0; j < n; j++){
                if (i + BLK - 1 < m){
                    double blk_items[BLK] = {0};
                    for (int q=0; q<k; q++){
                        for (int ii=0; ii<BLK;ii++){ // Calculate elements block [i,i+blk)
                            blk_items[ii] += A[i+ii][q] * B[q][j];
                        }
                    }
                    for (int sii=0;sii<BLK;sii++){
                        C[i+sii][j] = blk_items[sii];
                    }
                }else{ // elements in the (smaller) last block
                    // version 1
                    for (int ii=0;ii<(m%BLK);ii++){
                        double sum = 0;
                        for (int q=0; q<k; q++){
                            sum += A[i+ii][q] * B[q][j];
                        }
                        C[i+ii][j] = sum;
                    }
                } // END IF
            }
        }// END TARGET PARALLEL FOR
    } // END SPLITS


    // data from device
    #pragma omp target exit data nowait map(release:A[:m][:k],B[:k][:n]) map(from:C[:m][0:n])


    // Timing calculations
    comp_time= omp_get_wtime() - comp_time_s;
    end_t = omp_get_wtime();
    dataoff_time = (end_t - start_t) - comp_time;
    
    // Save results to file
    char myString[100]; // Allocate memory for myString array
    sprintf(myString, "%d, %f, %f\n",m, comp_time*1000, dataoff_time*1000);
    char filename[100] = "results/timings/timing_asy_offload.txt";
    saveToFile(myString, filename);
    // printf("%s", myString);

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