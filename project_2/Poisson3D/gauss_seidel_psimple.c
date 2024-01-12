/* gauss_seidel.c - Poisson problem in 3d
 *
 */
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <stdbool.h>
#include "cube_utils.h"


void compute_u(double ***u, double ***f, int N, double *diff_avg)
{

    double u_old;
    double h = (double)1 / 6;
    double delta = 2.0 / (double)N;
    double sqr_diff_acum=0;

    #pragma omp parallel for ordered(2) private(u_old) shared(u, f, N, diff_avg, h, delta) reduction(+:sqr_diff_acum) schedule(static,1)
    for (int i = 1; i < (N-1);i++){
        for (int j = 1; j < (N-1); j++){
            #pragma omp ordered \
                depend(sink: i,j-1) \
                depend(sink: i-1,j)
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
                sqr_diff_acum += (u_old - u[i][j][k]) * (u_old - u[i][j][k]);
            }
        #pragma omp ordered depend(source)
        }
    } // END PARALLELIZED FOR

    *diff_avg = sqrt(sqr_diff_acum);
}

void
gauss_seidel(double*** u, double*** f, int N, int iter_max, int *p_inter, double tolerance, bool verbose) {
    // simple parallel version

    int iter=0;

    double diff_avg=999;
    
    while (iter<iter_max && diff_avg>tolerance)
    {
            compute_u(u,f, N, &diff_avg);

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

