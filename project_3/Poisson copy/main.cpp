
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>
#include <math.h>
#include "init.h"
#include "jacobi.h"


#define N_DEFAULT 100

int main(int argc, char *argv[]) {


    /* get the paramters from the command line */
    int N         = atoi(argv[1]);	// grid size
    int iter_max  = atoi(argv[2]);  // max. no. of iterations
    // double tolerance = atof(argv[3]);  // tolerance
    double start_T   = atof(argv[3]);  // start T for all inner grid points
    int thread_num = atoi(argv[4]);  // number of threads

    double 	***u = NULL;
    double 	***output_u = NULL;
    double  ***f =  NULL;

    // Init the u cube
    init_cube(u, N, start_T); // TODO: add sanity check of high temp in corners
    // init force
    init_force(f, N);

    jacobi(u, output_u, f, N, iter_max);

    return 0;
}



