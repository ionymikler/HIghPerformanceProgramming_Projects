/* main.c - Poisson problem in 3D
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

#define N_DEFAULT 100

int
main(int argc, char *argv[]) {

    // Default parameters
    int 	N = N_DEFAULT;
    int 	iter_max = 5000;
    double	tolerance = 1e-5;
    double	start_T = 10; // mid of 0 and 20
    int		output_type = 0;
    bool    verbose = false;
    int     thread_num = 1;

    char	*output_prefix = "poisson_res";
    char    *output_ext    = "";
    char	output_filename[FILENAME_MAX];
    double 	***u = NULL;
    double 	***output_u = NULL;
    double  ***f =  NULL;


    /* get the paramters from the command line */
    N         = atoi(argv[1]);	// grid size
    iter_max  = atoi(argv[2]);  // max. no. of iterations
    tolerance = atof(argv[3]);  // tolerance
    start_T   = atof(argv[4]);  // start T for all inner grid points
    thread_num = atoi(argv[5]);  // number of threads
    verbose   = atoi(argv[6]);  // verbose output
    if (argc == 8) {
	output_type = atoi(argv[7]);  // ouput type
    }

    int t_num=4;
    omp_set_num_threads(t_num);

    // print parameters:
    if (verbose){
        printf("-- Poisson solver ---\n");
        print_params(N, iter_max, tolerance, start_T, thread_num, verbose);
        
        printf("Number of threads: %d\n",t_num);
    }

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
    output_prefix = "jacobi_res";
    #else
    output_prefix = "gauss_seidel_res";
    #endif

    // Init the u cube
    // TODO: add sanity check of high temp in corners
    init_cube(u, N, start_T);
    
    // init force
    init_force(f, N);

    if (verbose){printf("\nrunning solver\n");}
    // TODO: make into function
    double time_start,time_end;
    time_start = omp_get_wtime();
    #ifdef _JACOBI
    if (verbose){printf("runnig Jacobi\n");}
    init_cube(output_u, N, start_T);
    jacobi(u, output_u, f, N, iter_max, tolerance, verbose);
    #else
    if (verbose){printf("runnig Gauss-Seidel\n");}
    gauss_seidel(u,f, N,iter_max, tolerance, verbose);
    #endif
    time_end = omp_get_wtime();
    double time_total = (time_end - time_start);


    // TODO: make into function
    // calculate MFLOPS and MLUP/s
    if (verbose){
        printf("\nsolver done\n");
        print_params(N, iter_max, tolerance, start_T, thread_num, verbose);
        printf("wall time: %f\n",time_total);

        printf("Sanity check");
        sum_u(u,N);

    }else{
        // to logfile
        #ifdef _JACOBI
            char *algo = "Jacobi";
        #else
            char *algo = "Gauss-Seidel";
        #endif
        printf("%s,%d,%d,%d,%f,%f\n",
        algo, N, thread_num, iter_max, tolerance, time_total);
    }

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
