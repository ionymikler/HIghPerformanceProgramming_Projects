/* jacobi.c - Poisson problem in 3d
 * 
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

void
jacobi(double*** input, double*** output, double*** f, int N, int iter_max) {
    printf("Runnin g jacobi complex: \n");

    // delta = 2 / N where N is the number of points 
    int i, j, k;
    double h = 1/6.0;
    double time_start,time_end;
    double ***temp;
    time_start = omp_get_wtime();

    int steps = 0;

    for (steps=0; steps < iter_max; steps++){
        #pragma omp parallel shared(N,h, input,f,output,steps) private(i,j,k)
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
                    }
                }
            }
            temp = output;
            output = input;
            input = temp;
        }
        steps++;
    }
    // char *reason = steps==iter_max ? "max iterations reached": "tolerance reached";

    time_end = omp_get_wtime();
    double time_total = (time_end - time_start);

    printf("iter %d ", steps); // steps


    double MLUP = pow(N-2,3)*steps*pow(10,-6);
    double FLOPS = MLUP * 10/time_total;
    int thread = omp_get_max_threads();

    FILE *fptr = fopen("results.txt","a");
    fprintf(fptr, "%d ", N); // grid
    fprintf(fptr, "%f ", time_total); // time
    fprintf(fptr, "%d ", steps); // steps
    fprintf(fptr, "%f ", MLUP); // MLUP
    fprintf(fptr, "%f ", FLOPS); // MFLOPS
    fprintf(fptr, "%d ", N*N*N*8*2); // memory bytes
    fprintf(fptr, "%d ", thread); // grid

    fprintf(fptr, "\n");
}
