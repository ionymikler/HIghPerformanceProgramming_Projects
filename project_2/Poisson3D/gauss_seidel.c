/* gauss_seidel.c - Poisson problem in 3d
 *
 */
#include <stdio.h>
#include <math.h>
#include "cube_utils.h"


void
gauss_seidel(double*** u, double*** f, int N, int iter_max, double tolerance) {
    /*
        NOTE: length of cube is from -1 to 1, so 2
        delta is length/N -> 2/N
    */ 

    double delta = 2.0 / (double)N;
    double h = 1/6;
    double u_old;
    double diff_acum=999;
    int iter =0;

    // Loop through Step
    while (iter<iter_max && diff_acum>tolerance)
    {   
        diff_acum = 0;
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

                diff_acum += (u_old - u[i][j][k]) * (u_old - u[i][j][k]);
            }   
            }
        }
    }
    printf("--- Iterations stopped ---");
    printf("Iteration: %d, diff_acum: %f", iter, diff_acum);
}

