/* gauss_seidel.c - Poisson problem in 3d
 *
 */
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include "cube_utils.h"


void compute_u(double ***u, double ***f, \
    int *N, double *u_old, double *h, double *delta, double *diff_avg, double *sqr_diff_acum)
{
    int t_id = omp_get_thread_num();
    printf("T_id: %d. Compute_u\n", t_id);
    #pragma omp for ordered(2) \
        private(u_old) \
        // firstprivate(sqr_diff_acum)
    for (int i = 1; i < (*N-1);i++){
        for (int j = 1; j < (*N-1); j++){
            #pragma omp ordered \
                depend(sink: i,j-1) \
                depend(sink: i-1,j)
            for (int k = 1; k < (*N-1); k++){
                *u_old = u[i][j][k];

                u[i][j][k] = *h*(\
                    u[i+1][j][k] + \
                    u[i-1][j][k] + \
                    u[i][j+1][k] + \
                    u[i][j-1][k] + \
                    u[i][j][k+1] + \
                    u[i][j][k-1] + \
                    *delta * *delta * f[i][j][k]
                );
                *sqr_diff_acum += (*u_old - u[i][j][k]) * (*u_old - u[i][j][k]);
                printf("t_id: %d, u_old: %f",t_id, u_old);
            }
        #pragma omp ordered depend(source)
        }
    }

    // average difference
    double Nm2p3 = (*N-2)*(*N-2)*(*N-2); // N-2 to the power of three
    *diff_avg = sqrt(*sqr_diff_acum/Nm2p3);
}

void
gauss_seidel(double*** u, double*** f, int N, int iter_max, double tolerance) {
    /*
        NOTE: length of cube is from -1 to 1, so 2
        delta is length/N -> 2/N
    */

    printf("%s\n","running gs");
    double time_start,time_end;
    double delta = 2.0 / (double)N;
    double h = (double)1 / 6;
    double u_old;
    int iter=0;
    time_start = omp_get_wtime();

    // TODO: parallelize the while
    #pragma omp parallel default(none) \
        shared(iter, delta, h, u, f, N, iter_max, tolerance) \
        private(u_old)
    {
        #pragma omp single
        {
            printf("Single region\n");
            double diff_avg=999;
            while (iter<iter_max && diff_avg>tolerance)
            {
                #pragma omp task
                {
                    printf("Parallelized region\n");
                    printf("Num threads: %d\n", omp_get_num_threads());

                    double sqr_diff_acum = 0;
                    compute_u(u,f,&N,&u_old, &h, &delta, &diff_avg, &sqr_diff_acum);

                    if (iter % 100 == 0){
                        printf("iter: %d, diff_avg: %f\n",iter, diff_avg);
                    }
                    iter++;
                }// END OF TASK
            }
            
            
            char *reason = iter==iter_max ? "max iterations reached": "tolerance reached";
            
            // Printing of results
            printf("\n--- Iterations stopped ---\n");
            printf("reason: %s\n",reason);
            printf("Iteration: %d, diff_avg: %f\n", iter, diff_avg);
        } // END OF SINGLE
    } // END OF PARALLELIZATION

    time_end = omp_get_wtime();
    double time_total = (time_end - time_start);
    printf("wall time: %f\n",time_total); // deal with this for parallel
}

