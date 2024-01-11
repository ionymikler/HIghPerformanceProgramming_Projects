/* jacobi.c - Poisson problem in 3d
 * 
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

void
jacobi(double*** input, double*** output, double*** f, int N, int iter_max, double tolerance) {
    // delta = 2 / N where N is the number of points 
    double delta = 2.0 / (double)N;
    double dif = 10000.0;
    double h = 1/6.0;

    int steps = 0;
    while ( steps < iter_max && dif > tolerance){
        dif = 0;
         for (int i = 1; i < (N-1);i++){
            for (int j = 1; j < (N-1); j++){
                for (int k = 1; k < (N-1); k++){
                // calculate new value
                    output[i][j][k] = h * (input[i-1][j][k] + 
                                    input[i+1][j][k] +
                                    input[i][j-1][k] +
                                    input[i][j+1][k] + 
                                    input[i][j][k-1] +
                                    input[i][j][k+1] +
                                    delta * delta * f[i][j][k]);
                                                    
                
                    dif += (output[i][j][k] - input[i][j][k]) * (output[i][j][k] - input[i][j][k]);
                }
            }
        }
        dif = dif / ((N - 2) * (N - 2) * (N - 2));
        dif = sqrt(dif);
        // printf("dif: %lf\n", dif);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                for (int k = 0; k < N; k++) {
                    input[i][j][k] = output[i][j][k];
                }
            }
        }
        if (steps % 100 == 0){
            printf("steps: %d, dif: %f\n",steps, dif);
        }
        steps++;
    }
    char *reason = steps==iter_max ? "max iterations reached": "tolerance reached";
    
    printf("--- Iterations stopped ---\n");
    printf("reason: %s\n",reason);
    printf("Iteration: %d, dif: %f\n", steps, dif);
}
