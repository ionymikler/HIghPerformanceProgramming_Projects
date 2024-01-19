#ifndef __ALLOC_3D
#define __ALLOC_3D

double ***malloc_3d(int m, int n, int k);
double ***malloc_3d_device(int m, int n, int k, double **data, int dev_num);

#define HAS_FREE_3D
void free_3d(double ***array3D);

#endif /* __ALLOC_3D */
