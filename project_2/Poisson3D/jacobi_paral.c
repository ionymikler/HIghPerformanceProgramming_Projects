/* jacobi.c - Poisson problem in 3d
 * 
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

void
jacobi(double*** input, double*** output, double*** f, int N, int iter_max, double tolerance) {
    printf("Runnin g jacobi complex: \n");

    // delta = 2 / N where N is the number of points 
    double delta = 2.0 / (double)N;
    int i, j, k;
    double d;
    double dif = 10000.0;
    double h = 1/6.0;
    double time_start,time_end;
    double tolerance1 = tolerance*tolerance;
    time_start = omp_get_wtime();

    int steps = 0;
    while ( steps < iter_max && dif > tolerance1){
        dif = 0;
        #pragma omp parallel reduction(+ : dif) shared(N,h, input,f,output,delta,steps) private(i,j,k,d)
        {
            #pragma omp for
            for (i = 1; i < (N-1);i++){
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
                        dif += d*d;
                    }
                }
            } //implicit barrier here
            #pragma omp for
            for (i = 0; i < N; i++) {
                for (j  = 0; j < N; j++) {
                    for ( k = 0; k < N; k++) {
                        input[i][j][k] = output[i][j][k];
                    }
                }
            }//implicit barrier here
        } // end of parralized section
        steps++;
    }
    char *reason = steps==iter_max ? "max iterations reached": "tolerance reached";

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
