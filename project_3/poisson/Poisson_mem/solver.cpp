/* solver.c - Poisson problem in 3D
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "solver.h"
#include "jacobi_memcpy.h"


void solve(double ***u, double ***output_u, double ***f, int N, int iter_max, int *p_iter, double tolerance, bool verbose){
    // print("iter: %d\n", *iter);s
    jacobi(u, output_u, f, N, iter_max, N, tolerance);
}