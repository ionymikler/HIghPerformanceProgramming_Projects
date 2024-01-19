/* jacobi.c - Poisson problem in 3d */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "alloc3d.h"
void
jacobi(double ***u, double ***u_d0, double ***u_d1, double *u_data_d0, double *u_data_d1,
       double ***output_u, double ***output_u_d0,double ***output_u_d1,double *output_data_d0, double *output_data_d1,
       double ***f,double ***f_d0,double ***f_d1, double *f_data_d0, double *f_data_d1,
       int N, int iter_max, double tolerance) {

    printf("Running jacobi complex: \n");
    double delta = 2.0 / (double)N;
    int i, j, k;
    double ***temp;
    double h = 1/6.0;
    double time_start,time_end;
    int len = N;
    time_start = omp_get_wtime();
    int steps = 0;
  
    printf("Start mecmcpy: \n");
    //data pointer 
    omp_target_memcpy(f_data_d0,**f,N/2*N*N*sizeof(double),0,0,0, omp_get_initial_device());
    omp_target_memcpy(u_data_d0,**u,N/2*N*N*sizeof(double),0,0,0, omp_get_initial_device());
    omp_target_memcpy(output_data_d0,**output_u,N/2*N*N*sizeof(double),0,0,0, omp_get_initial_device());
    printf("GPU 1 memcpy OK \n");

    omp_target_memcpy(f_data_d1,**f,N/2*N*N*sizeof(double),0,N/2*N*N*sizeof(double),1, omp_get_initial_device());
    omp_target_memcpy(u_data_d1,**u,N/2*N*N*sizeof(double),0,N/2*N*N*sizeof(double),1, omp_get_initial_device());
    omp_target_memcpy(output_data_d1,**output_u,N/2*N*N*sizeof(double),0,N/2*N*N,1, omp_get_initial_device());
    printf("GPU 2 memcpy OK \n");
    
    while ( steps < iter_max){
        //Update the lower half
        // u_d0 and u_d1
      #pragma omp target teams distribute parallel for collapse(3) nowait \
                private(i,j,k)\
                is_device_ptr(u_d0, output_u_d0, f_d0, u_d1) device(0)
            for (i = 1; i < (N/2);i++){
                for ( j = 1; j < (N-1); j++){
                    for ( k = 1; k < (N-1); k++){
                    // calculate new value
                        if (i < (N/2-1)){
                            output_u_d0[i][j][k] = h * (u_d0[i-1][j][k] + 
                                            u_d0[i+1][j][k] +
                                            u_d0[i][j-1][k] +
                                            u_d0[i][j+1][k] + 
                                            u_d0[i][j][k-1] +
                                            u_d0[i][j][k+1] +
                                            f_d0[i][j][k]);
                        }else{
                            output_u_d0[i][j][k] = h * (u_d0[i-1][j][k] + 
                                            u_d1[0][j][k] +
                                            u_d0[i][j-1][k] +
                                            u_d0[i][j+1][k] + 
                                            u_d0[i][j][k-1] +
                                            u_d0[i][j][k+1] +
                                            f_d0[i][j][k]);

                        }
                    }
                } 
            } //Implicit boundary

            //Update the upper half
            #pragma omp target teams distribute parallel for collapse(3) nowait \
                private(i,j,k)\
                is_device_ptr(u_d0, u_d1, output_u_d1, f_d1) device(1)
            for (i = 0; i < (N/2-1);i++){
                for ( j = 1; j < (N-1); j++){
                    for ( k = 1; k < (N-1); k++){
                    // calculate new value
                        if (i > 0){
                            output_u_d1[i][j][k] = h * (u_d1[i-1][j][k] + 
                                            u_d1[i+1][j][k] +
                                            u_d1[i][j-1][k] +
                                            u_d1[i][j+1][k]  + 
                                            u_d1[i][j][k-1] +
                                            u_d1[i][j][k+1] +
                                            f_d1[i][j][k]);                     
                        }else{
                            output_u_d1[i][j][k] = h * (u_d0[N/2-1][j][k]+ 
                                            u_d1[i+1][j][k] +
                                            u_d1[i][j-1][k] +
                                            u_d1[i][j+1][k] + 
                                            u_d1[i][j][k-1] +
                                            u_d1[i][j][k+1] +
                                            f_d1[i][j][k]);

                        }
                    }
                } 
            }
        // Wait for both loops to finish
        #pragma omp taskwait
    

        // Readjust the pointers 
        temp = output_u_d0;
        output_u_d0 = u_d0; 
        u_d0 = temp;
        temp = output_u_d1;
        output_u_d1 = u_d1; 
        u_d1 = temp;

        steps++;
        
        if (steps%100==0) printf("Loop runs %d\n",steps );
    }
    printf("Loop terminated \n");
    // Check if the arrays are swapped
    if (steps % 2 == 1){
        u_d0 = output_u_d0;
        u_d1 = output_u_d1;
    }
    
    // Copy data back to device:
    omp_target_memcpy(**u,u_data_d0,N/2*N*N*sizeof(double),0,0,omp_get_initial_device(),0);
    printf("DtH terminated 0\n");
    omp_target_memcpy(**u,u_data_d1,N/2*N*N*sizeof(double),N/2*N*N*sizeof(double),0,omp_get_initial_device(),1);
    printf("DtH terminated 1 \n");

    time_end = omp_get_wtime();
    double time_total = (time_end - time_start);

    printf("iter %d ", steps); // steps


    double MLUP = pow(N-2,3)*steps*pow(10,-6)/time_total;
    double FLOPS = MLUP * 7;
    int thread = omp_get_max_threads();

    FILE *fptr = fopen("results_mem_2D.txt","a");
    fprintf(fptr, "%d ", N); // grid
    fprintf(fptr, "%f ", time_total); // time
    fprintf(fptr, "%d ", steps); // steps
    fprintf(fptr, "%f ", MLUP); // MLUP
    fprintf(fptr, "%f ", FLOPS); // MFLOPS
    fprintf(fptr, "%d ", N*N*N*8*2); // memory bytes
    fprintf(fptr, "%d ", thread); // grid

    fprintf(fptr, "\n");
}