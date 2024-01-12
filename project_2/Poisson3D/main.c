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
#include "solver.h"

// #ifdef _JACOBI
// #include "jacobi.h"
// #endif

// #ifdef _GAUSS_SEIDEL
// #include "gauss_seidel.h"
// #endif

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
    
    // printf("Num threads: %d\n",omp_get_max_threads());

    // print parameters:
    if (verbose){
        printf("-- Poisson solver ---\n");
        print_params(N, iter_max, tolerance, start_T, thread_num, verbose, output_type);        
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
    init_cube(output_u, N, start_T);
    #endif

    // Init the u cube
    init_cube(u, N, start_T); // TODO: add sanity check of high temp in corners
    // init force
    init_force(f, N);

    if (verbose){printf("\nrunning solver\n");}
    double time_start,time_end;
    time_start = omp_get_wtime();
    // All the magic happens here
    int iter = 0;
    int *p_iter = &iter;
    solve(u, output_u, f, N, iter_max, p_iter, tolerance, verbose);
    // All the magic ends here
    time_end = omp_get_wtime();
    double time_total = (time_end - time_start);


    // RESULTS AREA
    int tot_updated_points = (N-2)*(N-2)*(N-2);
    int flops_total, lattice_ups_total; // will be calculated depending on iterations
    double MFlops, MLUPs; // per second, will be calculated depending on iterations

    int flops_per_lattice_per_iter;
    #ifdef _JACOBI
        char *algo_name = "Jacobi";
        flops_per_lattice_per_iter = 7; 
    #else
        char  *algo_name = "Gauss-Seidel";
        flops_per_lattice_per_iter = 10;
    #endif

    // calculate MFLOPS and MLUP/s
    lattice_ups_total = *p_iter * tot_updated_points;
    flops_total = lattice_ups_total * flops_per_lattice_per_iter;

    MFlops = flops_total / (time_total * 1e6);
    MLUPs = lattice_ups_total / (time_total * 1e6);

    if (verbose){
        printf("\nsolver done\n");
        print_params(N, iter_max, tolerance, start_T, thread_num, verbose, output_type);

        printf("\n--- Results ---\n");
        printf("Iterations: %d\n",iter);
        printf("MFlops/s: %f\n",MFlops);
        printf("MLUPs/s: %f\n",MLUPs);
        printf("wall time: %f\n",time_total);

        printf("Sanity check");
        sum_u(u,N);

    }else{
        // to logfile
        printf("%s, %d, %d, %f, %d, %f, %f\n",
        algo_name, N, thread_num, time_total, iter, MFlops, MLUPs);
    }

    // dump results if wanted 
    char	output_filename[FILENAME_MAX];
    char	output_prefix[50];
    char    *output_ext    = "";
    sprintf(output_prefix,"%s_%s",algo_name, "res");
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
