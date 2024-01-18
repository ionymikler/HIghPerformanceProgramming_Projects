
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>
#include <math.h>
#include "jacobi.h"
#include "print.h"
#include "init.h"
#include "alloc3d.h"
#include <omp.h>


int main(int argc, char *argv[]) {
    /* get the paramters from the command line */
    int N         = atoi(argv[1]);	// grid size
    int iter_max  = atoi(argv[2]);  // max. no. of iterations
    double start_T   = atof(argv[3]);  // start T for all inner grid points
    double 	***u = NULL;
    double 	***output_u = NULL;
    double  ***f =  NULL;

    u = malloc_3d(N, N, N);
    f = malloc_3d(N, N, N);
    output_u = malloc_3d(N, N, N);

    // Init the u cube
    init_cube(u, N, start_T); // TODO: add sanity check of high temp in corners
    init_cube(output_u, N, start_T); // TODO: add sanity check of high temp in corners

    // init force
    init_force(f, N);

    jacobi(u, output_u, f, N, iter_max);

    char	output_filename[FILENAME_MAX];
    sprintf(output_filename, "%s_%d%s", "ExampleJaco", N, ".vtk");
    print_vtk(output_filename, N, output_u);

    return 0;
}



