/* gauss_seidel.c - Poisson problem in 3d
 *
 */
#include <math.h>
#include "cube_utils.h"


double***
gauss_seidel(double*** u, double*** output, double*** f, int N, int iter_max, double tolerance) {
    /*
        NOTE: length of cube is from -1 to 1, so 2
        delta is length/N -> 2/N
    */ 

    double delta = 2.0 / (double)N;
    double dif = 10000;
    double min_dif = .01;
    int max_step = 100;
    
    // Create the first 3D matrix, all values are initialized to zero
    int range_all[6] = {0,N-1,0,N-1,0,N-1};
    // populate_range(u,range_all,0);

    // populate_walls(u, N, 20);
    // // Initialize the boundary values for the output, these will not be overwritten in the subsequent loops 
    // for (int i = 0; i < N; i++){
    //     for (int j = 0; j < N; j++){
    //         output[0][i][j] = 20;
    //         output[N-1][i][j] = 20;
    //         output[i][N-1][j] = 20;
    //         output[i][j][0] = 20;
    //         output[i][j][N-1] = 20;
    //         output[i][0][j] = 0;
    //     }
    // }
    // boundary (wall)  conditions for u as well 
    // u = output;

    // // Loop through Steps
    // for (int steps = 0; steps < iter_max; steps++){ 
    //      for (int i = 1; i < (N-1);i++){
    //         for (int j = 1; j < (N-1); j++){
    //             for (int k = 1; k < (N-1); k++){
    //             // Calculate new value - rewritten for readability.
    //                 output[i][j][k] = (u[i-1][j][k] + 
    //                                 u[i+1][j][k] +
    //                                 u[i][j-1][k] +
    //                                 u[i][j+1][k] + 
    //                                 u[i][j][k-1] +
    //                                 u[i][j][k+1] +
    //                                 pow(delta,2) * f[i][j][k])/ 6.0;

    //                 // Add norm
    //             }
    //         }
    //     }
    //     // Calculate the difference criteria
        
    //     double dif = 100.0;//abs(u[0][0][0] - output[0][0][0]);
    //     // Make new u
    //     u = output;

    // }
    // return u;
}

