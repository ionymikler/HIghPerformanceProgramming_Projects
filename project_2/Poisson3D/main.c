/* main.c - Poisson problem in 3D
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include "alloc3d.h"
#include "cube_utils.h"
#include "print.h"

#ifdef _JACOBI
#include "jacobi.h"
#endif

#ifdef _GAUSS_SEIDEL
#include "gauss_seidel.h"
#endif

#define N_DEFAULT 10

int
main(int argc, char *argv[]) {

    // Default parameters
    int 	N = N_DEFAULT;
    int 	iter_max = 5000;
    double	tolerance = 1e-5;
    double	start_T = 10; // mid of 0 and 20
    int		output_type = 4;

    char	*output_prefix = "poisson_res";
    char    *output_ext    = "";
    char	output_filename[FILENAME_MAX];
    double 	***u = NULL;
    double 	***output_u = NULL;
    double  ***f =  NULL;


    /* get the paramters from the command line */
    N         = atoi(argv[1]);	// grid size
    // iter_max  = atoi(argv[2]);  // max. no. of iterations
    // tolerance = atof(argv[3]);  // tolerance
    // start_T   = atof(argv[4]);  // start T for all inner grid points
    // if (argc == 6) {
	// output_type = atoi(argv[5]);  // ouput type
    // }

    int t_num=2;
    omp_set_num_threads(t_num);

    // print parameters:
    printf("-- Poisson solver ---\n");
    printf("-- solver Parameters---\n");
    printf("N: %d\n",N);
    printf("iter max: %d\n", iter_max);
    printf("tolerance: %f\n", tolerance);
    
    printf("-- solver Parameters---\n");
    printf("thread number: %d\n",t_num);

    // allocate memory
    if ( (u = malloc_3d(N, N, N)) == NULL ) {
        perror("array u: allocation failed");
        exit(-1);
    }
    // Create cube u and force f
    if ( (f = malloc_3d(N, N, N)) == NULL ) {
        perror("array f: allocation failed");
        exit(-1);
    }

    #ifdef _JACOBI
    if ( (output_u = malloc_3d(N, N, N)) == NULL ) {
        perror("array u: allocation failed");
        exit(-1);
    }
    #endif

    // Init the u cube
    init_cube(u, N, start_T);
    
    // init force
    init_force(f, N);

    printf("\nrunning solver\n");
    #ifdef _JACOBI
    init_cube(output_u, N, start_T);
    jacobi(u, output_u, f, N, iter_max, tolerance);
    #else
    gauss_seidel(u,f, N,iter_max, tolerance);
    #endif
    printf("\nsolver done\n");

    // dump  results if wanted 
    switch(output_type) {
	case 0:
	    // no output at all
	    break;
	case 3:
	    output_ext = ".bin";
	    sprintf(output_filename, "%s_%d%s", output_prefix, N, output_ext);
	    fprintf(stderr, "Write binary dump to %s: ", output_filename);
	    print_binary(output_filename, N, u);
	    break;
	case 4:
	    output_ext = ".vtk";
	    sprintf(output_filename, "%s_%d%s", output_prefix, N, output_ext);
	    fprintf(stderr, "Write VTK file to %s:\n", output_filename);
	    print_vtk(output_filename, N, u);
	    break;
	default:
	    fprintf(stderr, "Non-supported output type!\n");
	    break;
    }

    // de-allocate memory
    free_3d(u);
    free_3d(f);
    #ifdef _JACOBI
    free_3d(output_u);
    #endif

    return(0);
}
