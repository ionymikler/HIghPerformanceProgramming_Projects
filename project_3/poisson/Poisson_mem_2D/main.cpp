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
#include "jacobi_memcpy.h"

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



    cudaSetDevice(0);
    cudaDeviceEnablePeerAccess(1, 0); // (dev 1, future flag)
    cudaSetDevice(1);
    cudaDeviceEnablePeerAccess(0, 0); // (dev 0, future flag)
    cudaSetDevice(0);


    double 	***u = NULL;
    double 	***output_u = NULL;
    double  ***f =  NULL;

    // Iniate pointers for data
    double *data_u0=NULL;
    double *data_f0=NULL;
    double *data_out0=NULL;
    double *data_u1=NULL;
    double *data_f1=NULL;
    double *data_out1=NULL;


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
    double ***u_d0 = malloc_3d_device(N/2,N,N, &data_u0,0);
    double ***output_u_d0 = malloc_3d_device(N/2,N,N, &data_out0,0);
    double ***f_d0 = malloc_3d_device(N/2,N,N,&data_f0,0);
    double ***u_d1 = malloc_3d_device(N/2,N,N, &data_u1,1);
    double ***output_u_d1 = malloc_3d_device(N/2,N,N, &data_out1,1);
    double ***f_d1 = malloc_3d_device(N/2,N,N,&data_f1,1);

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

    
    if ( (output_u = malloc_3d(N, N, N)) == NULL ) {
        perror("array u: allocation failed");
        exit(-1);
    }
    init_cube(output_u, N, start_T);

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
    jacobi(u, u_d0,u_d1, data_u0,data_u1,
           output_u, output_u_d0, output_u_d1,data_out0, data_out1,
           f,f_d0,f_d1, data_f0, data_f1,
           N, iter_max, tolerance);
    
    // All the magic ends here
    time_end = omp_get_wtime();
    double time_total = (time_end - time_start);


    // RESULTS AREA
    int tot_updated_points = (N-2)*(N-2)*(N-2);
    double MFlops_total, MLups_total; // will be calculated depending on iterations
    double MFlops_per_s, MLUPs_per_s; // per second, will be calculated depending on iterations

    int flops_per_lattice_per_iter;
    char *algo_name = "Jacobi";
    flops_per_lattice_per_iter = 7; 
    
    // calculate MFLOPS and MLUP/s
    double scl = tot_updated_points/(time_total * 1e6);

    MLups_total = *p_iter *  scl;

    MFlops_total = MLups_total * flops_per_lattice_per_iter;

    MFlops_per_s = MFlops_total;
    MLUPs_per_s = MLups_total;

    if (verbose){
        printf("\nsolver done\n");
        print_params(N, iter_max, tolerance, start_T, thread_num, verbose, output_type);

        printf("\n--- Results ---\n");
        printf("Iterations: %d\n",iter);
        printf("MFlops/s: %f\n",MFlops_per_s);
        printf("MLUPs/s: %f\n",MLUPs_per_s);
        printf("wall time: %f\n",time_total);

        printf("Sanity check");
        sum_u(u,N);

    }else{
        // to logfile
        printf("%d, %f, %d, %f, %f, %d\n",
            N, time_total, iter, MLUPs_per_s, MFlops_per_s, thread_num);
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
