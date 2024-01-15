/* solver.c - Poisson problem in 3D
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "solver.h"

#ifdef _JACOBI
#include "jacobi.h"
#endif

#ifdef _GAUSS_SEIDEL
#include "gauss_seidel.h"
#endif

void solve(double ***u, double ***output_u, double ***f, int N, int iter_max, int *p_iter, double tolerance, bool verbose){
    // print("iter: %d\n", *iter);
    #ifdef _JACOBI
    jacobi(u, output_u, f, N, iter_max, p_iter, tolerance, verbose);
    #else
    gauss_seidel(u, f, N, iter_max, p_iter, tolerance, verbose);
    #endif
}