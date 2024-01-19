/* jacobi.c - Poisson problem in 3d
 * 
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

void jacobi(double ***u, double *u_d, double ***output_u, double *output_u_d, double ***f, double *f_d,
       int N, int iter_max, double tolerance) {
    printf("Running jacobi complex: \n");
    double delta = 2.0 / (double)N;
    int i, j, k;
    double *temp;

    double h = 1/6.0;
    double time_start,time_end;
    double sq_tolerance = tolerance*tolerance;
    time_start = omp_get_wtime();
    int steps = 0;

    
    int dev_num = omp_get_default_device();

    // Copy initial conditions to device
    printf("Check 1\n");
    omp_target_memcpy(f_d,**f,N*N*N*sizeof(double),0,0,dev_num, omp_get_initial_device());
    omp_target_memcpy(u_d,**u,N*N*N*sizeof(double),0,0,dev_num, omp_get_initial_device());
    omp_target_memcpy(output_u_d,**output_u,N*N*N*sizeof(double),0,0,dev_num, omp_get_initial_device());

    while ( steps < iter_max){
        #pragma omp target teams distribute parallel for collapse(3) \
                default(shared) private(i,j,k)\
                is_device_ptr(u_d, output_u_d, f_d)
            for (i = 1; i < (N-1);i++){
                for ( j = 1; j < (N-1); j++){
                    for ( k = 1; k < (N-1); k++){
                    // calculate new value
                    // output_u_d is linearized.
                        output_u_d[i * N*N + j*N + k] = h * (u_d[(i-1) * N* N + j* N + k] + 
                                        u_d[(i+1) * N* N + j* N + k] +
                                        u_d[i*N*N + (j-1) * N + k] +
                                        u_d[i*N*N + (j+1) * N + k]  + 
                                        u_d[i*N*N + j*N + (k-1)] +
                                        u_d[i*N*N + j*N + (k+1)] +
                                        f_d[i * N*N + j*N + k]);
                    }
                } 
            } //Implicit boundary

        // Readjust the pointers 
        temp = output_u_d;
        output_u_d = u_d; 
        u_d = temp;

        steps++;
        if (steps%100 ==0) printf("Loop runs %d\n",steps );
    }

    // Check if the arrays are swapped
    if (steps % 2 == 1) u_d = output_u_d;

    omp_target_memcpy(**u,u_d,N*N*N*sizeof(double),0,0, omp_get_initial_device(), dev_num);

    time_end = omp_get_wtime();
    double time_total = (time_end - time_start);

    printf("iter %d ", steps); // steps


    double MLUP = pow(N-2,3)*steps*pow(10,-6);
    double FLOPS = MLUP * 10/time_total;
    int thread = omp_get_max_threads();

    FILE *fptr = fopen("results_mem.txt","a");
    fprintf(fptr, "%d ", N); // grid
    fprintf(fptr, "%f ", time_total); // time
    fprintf(fptr, "%d ", steps); // steps
    fprintf(fptr, "%f ", MLUP); // MLUP
    fprintf(fptr, "%f ", FLOPS); // MFLOPS
    fprintf(fptr, "%d ", N*N*N*8*2); // memory bytes
    fprintf(fptr, "%d ", thread); // grid

    fprintf(fptr, "\n");
}