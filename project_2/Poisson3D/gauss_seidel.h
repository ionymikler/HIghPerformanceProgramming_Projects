/* gauss_seidel.h - Poisson problem
 *
 */
#ifndef _GAUSS_SEIDEL_H
#define _GAUSS_SEIDEL_H
#include <stdbool.h>
void
gauss_seidel(double*** u, double*** f, int N, int iter_max, int *p_inter, double tolerance, bool verbose);

#endif
