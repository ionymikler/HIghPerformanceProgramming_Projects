#include "cube_utils.h"
#include <math.h>

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

void populate_walls(double ***p, int N, double temp){
    int i;
    // TODO: Make the 8 cube corners have a very very large value for sanity check
    // TODO: There must be a smarter way to do this
   for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            p[0][i][j] = temp;
            p[N-1][i][j] = temp;
            p[i][N-1][j] = temp;
            p[i][j][0] = temp;
            p[i][j][N-1] = temp;
            p[i][0][j] = 0;
        }
    }
}

void init_cube(double ***p, int N){
    // fill all values to zero
    int range_all[6] = {0,N-1,0,N-1,0,N-1};
    populate_range(p,range_all,0);

    populate_walls(p, N, 20);
}

void init_force(double ***f, int N){
    // Calculate extend of the heater location
    int rad_x_s=0, rad_x_e=floor(5*N/16);
    int rad_y_s=0, rad_y_e=floor(N/4);
    int rad_z_s=ceil(N/2), rad_z_e=floor(N/2);
    int radiator_range[6] = {rad_x_s, rad_x_e, rad_y_s, rad_y_e, rad_z_s, rad_z_e};
    populate_range(f,radiator_range,200);
}