#include <stdlib.h>

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

void
free_3d(double ***p) {
    free(p[0][0]);
    free(p);
}

void populate_range(double ***p, int startsEnds[6], double val){
    int x_s = startsEnds[0];
    int x_e = startsEnds[1];

    int y_s = startsEnds[2];
    int y_e = startsEnds[3];
    
    int z_s = startsEnds[4];
    int z_e = startsEnds[5];

    for (int i=x_s;i<x_e;i++){
        for (int j=y_s; j<y_e; j++){
            for (int k=z_s; k<z_e; k++){
                p[i][j][k] = val;
            }
        }
    }   
}

// void populate_range(double ***p, double val){
//     p[1][1][1] = val;
// }