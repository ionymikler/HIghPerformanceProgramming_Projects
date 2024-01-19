#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void jacobi(double ***u, double ***u_d0, double ***u_d1, double *u_data_d0, double *u_data_d1,
       double ***output_u, double ***output_u_d0,double ***output_u_d1,double *output_data_d0, double *output_data_d1,
       double ***f,double ***f_d0,double ***f_d1, double *f_data_d0, double *f_data_d1,
       int N, int iter_max, double tolerance);