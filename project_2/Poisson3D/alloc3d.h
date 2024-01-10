#ifndef __ALLOC_3D
#define __ALLOC_3D

double ***malloc_3d(int m, int n, int k);
void populate_range(double ***p, int startsEnds[6], double val);
// void populate_range(double ***p, double val);

#define HAS_FREE_3D
void free_3d(double ***array3D);

#endif /* __ALLOC_3D */
