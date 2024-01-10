#include <stdio.h>
#include <stdlib.h>
#include "mandel.h"
#include "writepng.h"
#include <omp.h>



int
main(int argc, char *argv[]) {

    int   width, height;
    int	  max_iter;
    int   *image;

    width    = 4601;
    height   = 4601;
    max_iter = 400;

    // command line argument sets the dimensions of the image
    if ( argc == 2 ) width = height = atoi(argv[1]);

    image = (int *)malloc( width * height * sizeof(int));
    if ( image == NULL ) {
       fprintf(stderr, "memory allocation failed!\n");
       return(1);
    }
    
    double start_time = omp_get_wtime();
    mandel(width, height, image, max_iter);
    double end_time = omp_get_wtime();

    printf("Time is %lf seconds\n", end_time - start_time);

    writepng("mandelbrot.png", image, width, height);

    return(0);
}
