#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void jacobi(double ***u, double *u_d, double ***output_u, double *output_u_d, double ***f, double *f_d, int N, int iter_max, double tolerance);