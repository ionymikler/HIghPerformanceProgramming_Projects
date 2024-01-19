/* jacobi.c - Poisson problem in 3d
 * 
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

void
jacobi(double*** input, double*** output, double*** f, int N, int iter_max, double tolerance) {
    printf("Runnin g jacobi: \n");

    int i, j, k;
    int step=0;
    double h = 1/6.0;
    double time_start,time_end;
    double ***temp;
    double d;
    double dif = 10000.0;
    double tolerance1 = tolerance*tolerance;

    time_start = omp_get_wtime();
    
    #pragma omp target data map(tofrom: output[0:N][0:N][0:N]) map(to:input[0:N][0:N][0:N], f[0:N][0:N][0:N])
    {
    double timer_S = omp_get_wtime();
    while (step < iter_max && dif > tolerance1){ 
        dif = 0.;
        #pragma omp target teams distribute parallel for collapse (3) reduction(+ : dif)// map(tofrom: dif)
        for (i = 1; i < (N-1);i++){
            for ( j = 1; j < (N-1); j++){
                for ( k = 1; k < (N-1); k++){
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
        }
        temp = output;
        output = input;
        input = temp;
        step++;
    }
    }

    // if (step%2 != 0){
    //     output = input;
    // }

    time_end = omp_get_wtime();
    double time_total = (time_end - time_start);

    printf("%lf",time_total);

    double MLUP = pow(N-2,3)*iter_max*pow(10,-6)/time_total;
    double FLOPS = MLUP * 10/time_total;

    FILE *fptr = fopen("results.txt","a");
    fprintf(fptr, "%d ", N); // grid
    fprintf(fptr, "%f ", time_total); // time
    fprintf(fptr, "%d ", step); // steps
    fprintf(fptr, "%f ", MLUP); // MLUP
    fprintf(fptr, "%f ", FLOPS); // MFLOPS
    fprintf(fptr, "%d ", N*N*N*8*2); // memory bytes
    fprintf(fptr, "%f ", tolerance1); // memory bytes

    fprintf(fptr, "\n");
}
