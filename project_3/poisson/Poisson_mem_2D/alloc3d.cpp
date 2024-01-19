#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

double *** malloc_3d_device(int m, int n, int k, double **data, int dev_num){
    if (m <= 0 || n <= 0 || k <= 0)
        return NULL;


    double ***A = (double***) omp_target_alloc(m*sizeof(double **) +
                                               m*n * sizeof(double *), dev_num);
    if (A == NULL)
        return NULL;
    #pragma omp target is_device_ptr(A) device(dev_num)
    for(int i = 0; i < m; i++) {
        A[i] = (double **) A + m + i * n ;
    }

    double *a = (double*)omp_target_alloc(m*n*k * sizeof(double), dev_num);
    if (a == NULL) {
        omp_target_free(A, dev_num);
        return NULL;
    }


    #pragma omp target is_device_ptr(A, a) device(dev_num)
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n;j++)
            A[i][j] = a + (i * n * k) + (j * k);
        }

    *data = a; 
    return A;
}
  
void
free_3d(double ***p) {
    free(p[0][0]);
    free(p);
}

double ***
malloc_3d(int m, int n, int k) {

    if (m <= 0 || n <= 0 || k <= 0)
        return NULL;

    double ***p = (double***) malloc(m * sizeof(double **) +
                                     m * n * sizeof(double *));
    if (p == NULL) {
        return NULL;
    }

    for(int i = 0; i < m; i++) {
        p[i] = (double **) p + m + i * n ;
    }

    double *a = (double*) malloc(m * n * k * sizeof(double));
    if (a == NULL) {
	free(p);
	return NULL;
    }

    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            p[i][j] = a + (i * n * k) + (j * k);
        }
    }

    return p;
}
