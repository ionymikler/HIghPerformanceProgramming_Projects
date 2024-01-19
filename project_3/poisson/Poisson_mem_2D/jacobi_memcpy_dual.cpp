/* jacobi.c - Poisson problem in 3d */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

void
jacobi(double*** input, double*** output, double*** f, int N, int iter_max, double tolerance) {
    printf("Running jacobi complex: \n");
    double delta = 2.0 / (double)N;
    int i, j, k;
    double ***temp;

    double d;
    double dif_gpu0,dif_gpu1;
    double h = 1/6.0;
    double time_start,time_end;
    double sq_tolerance = tolerance*tolerance;
    time_start = omp_get_wtime();
    int steps = 0;

    cudaSetDevice(0);
    cudaDeviceEnablePeerAccess(1, 0); // (dev 1, future flag)
    cudaSetDevice(1);
    cudaDeviceEnablePeerAccess(0, 0); // (dev 0, future flag)
    cudaSetDevice(0);
    
    dif_gpu0 = 1000;
    while ( steps < iter_max && dif_gpu0 > sq_tolerance){
        // Reset counters
        dif_gpu0 = 0;
        dif_gpu1 = 0;

        //Update the lower half
        #pragma omp target teams distribute parallel for collapse(3) nowait \
                reduction(+ : dif_gpu0) default(shared) private(i,j,k,d)\
                is_device_ptr(input, output, f) device(0)
            for (i = 1; i < (N/2);i++){
                for ( j = 1; j < (N-1); j++){
                    for ( k = 1; k < (N-1); k++){
                    // calculate new value
                        output[i][j][k] = h * (input[i-1][j][k] + 
                                        input[i+1][j][k] +
                                        input[i][j-1][k] +
                                        input[i][j+1][k] + 
                                        input[i][j][k-1] +
                                        input[i][j][k+1] +
                                        f[i][j][k]);
                                                        
                        d = output[i][j][k] - input[i][j][k];
                        dif_gpu0 += d*d;
                    
                } 
            } //Implicit boundary

            //Update the upper half
            #pragma omp target teams distribute parallel for collapse(3) nowait \
            reduction(+ : dif_gpu1) default(shared) private(i,j,k,d)\
            is_device_ptr(input, output, f) device(1)
            for (i = N/2; i < (N-1);i++){
                for ( j = 1; j < (N-1); j++){
                    for ( k = 1; k < (N-1); k++){
                    // calculate new value
                        output[i][j][k] = h * (input[i-1][j][k] + 
                                        input[i+1][j][k] +
                                        input[i][j-1][k] +
                                        input[i][j+1][k] + 
                                        input[i][j][k-1] +
                                        input[i][j][k+1] +
                                        f[i][j][k]);
                                                        
                        d = output[i][j][k] - input[i][j][k];
                        dif_gpu1 += d*d;
                    }
                } 
            }
        // Wait for both loops to finish
        #pragma omp taskwait

        // Total tolerance
        dif_gpu0 += dif_gpu1;
        

        // Readjust the pointers 
        temp = output;
        output = input; 
        input = temp;

        steps++;
        
        if (steps == 100) printf("Loop runs %d\n",steps );
        if (steps == 100) printf("Tolerance: %lf\n",dif_gpu0);
    }

    // Check if the arrays are swapped
    if (steps % 2 == 1) input = output;

    time_end = omp_get_wtime();
    double time_total = (time_end - time_start);

    printf("iter %d ", steps); // steps


    double MLUP = pow(N-2,3)*steps*pow(10,-6);
    double FLOPS = MLUP * 10/time_total;
    int thread = omp_get_max_threads();

    FILE *fptr = fopen("results_thread_j_p.txt","a");
    fprintf(fptr, "%d ", N); // grid
    fprintf(fptr, "%f ", time_total); // time
    fprintf(fptr, "%d ", steps); // steps
    fprintf(fptr, "%f ", MLUP); // MLUP
    fprintf(fptr, "%f ", FLOPS); // MFLOPS
    fprintf(fptr, "%d ", N*N*N*8*2); // memory bytes
    fprintf(fptr, "%d ", thread); // grid

    fprintf(fptr, "\n");
}