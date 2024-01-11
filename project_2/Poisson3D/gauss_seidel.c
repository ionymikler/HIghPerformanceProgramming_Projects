/* gauss_seidel.c - Poisson problem in 3d
 *
 */
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include "cube_utils.h"


void
gauss_seidel(double*** u, double*** f, int N, int iter_max, double tolerance) {
    /*
        NOTE: length of cube is from -1 to 1, so 2
        delta is length/N -> 2/N
    */

    printf("runnig gs\n");
    double time_start,time_end;
    time_start = omp_get_wtime();

    double delta = 2.0 / (double)N;
    double h = (double)1 / 6;
    double u_old;
    double diff_acum=0,diff_avg=999;
    int iter =0;

    // Loop through Step
    while (iter<iter_max && diff_avg>tolerance)
    // while (iter<iter_max)
    {
        diff_acum = 0; // reset diff for every iteration
        diff_avg=999;
        for (int i = 1; i < (N-1);i++){
            for (int j = 1; j < (N-1); j++){
                for (int k = 1; k < (N-1); k++){
                    u_old = u[i][j][k];

                    u[i][j][k] = h*(\
                        u[i+1][j][k] + \
                        u[i-1][j][k] + \
                        u[i][j+1][k] + \
                        u[i][j-1][k] + \
                        u[i][j][k+1] + \
                        u[i][j][k-1] + \
                        delta * delta * f[i][j][k]
                    );
                    diff_acum += sqrt((u_old - u[i][j][k]) * (u_old - u[i][j][k]));
                }
            }
        }
        diff_avg = diff_acum/N;
        if (iter % 100 == 0){
            printf("iter: %d, diff_avg: %f\n",iter, diff_avg);
        }
        iter++;
    }
    char *reason = iter==iter_max ? "max iterations reached": "tolerance reached";
    time_end = omp_get_wtime();
    double time_total = (time_end - time_start);
    
    printf("--- Iterations stopped ---\n");
    printf("reason: %s\n",reason);
    printf("Iteration: %d, diff_avg: %f\n", iter, diff_avg);
    printf("wall time: %f\n",time_total);
}

