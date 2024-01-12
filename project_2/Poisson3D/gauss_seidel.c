/* gauss_seidel.c - Poisson problem in 3d
 *
 */
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include "cube_utils.h"


void
gauss_seidel(double*** u, double*** f, int N, int iter_max, int *p_inter, double tolerance, bool verbose) {
    // sequential version of gauss_seidel

    printf("runnig gs\n");

    double delta = 2.0 / (double)N;
    double h = (double)1 / 6;
    double u_old, diff;
    double sqr_diff_acum=0,diff_avg=999;
    double Nm2p3;
    int iter =0;

    // Loop through Step
    while (iter<iter_max && diff_avg>tolerance)
    {
        sqr_diff_acum = 0; // reset diff for every iteration
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
                        f[i][j][k]
                    );
                    diff = u_old - u[i][j][k];
                    sqr_diff_acum += diff*diff;
                }
            }
        }
        diff_avg = sqrt(sqr_diff_acum);
        iter++;
    }

    // Printing of results
    if (verbose){
        char *reason = iter==iter_max ? "max iterations reached": "tolerance reached";
        printf("\n--- Iterations stopped ---\n");
        printf("reason: %s\n",reason);
        printf("Iteration: %d, diff_avg: %f\n", iter, diff_avg);
    }
    *p_inter = iter;
}

