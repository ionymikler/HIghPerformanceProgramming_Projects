/* jacobi.h - Poisson problem 
 *
 * $Id: jacobi.h,v 1.1 2006/09/28 10:12:58 bd Exp bd $
 */

#ifndef _JACOBI_H
#define _JACOBI_H
#include <stdbool.h>

void jacobi(double***, double***, double***, int, int, int*, double, bool verbose);

#endif