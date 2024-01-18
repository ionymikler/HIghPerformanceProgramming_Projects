
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>
#include <math.h>
#include "jacobi_map.h"


#ifdef _JACOBI
#include "jacobi.h"
#endif

#ifdef _GAUSS_SEIDEL
#include "gauss_seidel.h"
#endif

#define N_DEFAULT 100

int main(int argc, char *argv[]) {


    /* get the paramters from the command line */
    int N         = atoi(argv[1]);	// grid size
    int iter_max  = atoi(argv[2]);  // max. no. of iterations
    double tolerance = atof(argv[3]);  // tolerance
    double start_T   = atof(argv[4]);  // start T for all inner grid points
    int thread_num = atoi(argv[5]);  // number of threads
    if (argc == 8) {
	double output_type = atoi(argv[7]);  // ouput type
    }

    // Init the u cube
    init_cube(u, N, start_T); // TODO: add sanity check of high temp in corners
    // init force
    init_force(f, N);


    #ifdef _JACOBI
    jacobi(u, output_u, f, N, iter_max, p_iter, tolerance, verbose);
    #else
    gauss_seidel(u, f, N, iter_max, p_iter, tolerance, verbose);
    #endif

    return 0;
}



