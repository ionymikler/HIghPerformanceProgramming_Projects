/* jacobi.c - Poisson problem in 3d
 * 
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

double***
jacobi(double*** input, double*** output, double*** f, int N, int iter_max, double tolerance) {
    //Delta 2 / N where N is the number of points 
    double delta = 2.0 / (double)N;
    double dif = 10000.0;
    double min_dif = .01;
    int max_step = 100;

    // Calculate extend of the heater location 
    int max_x_location = floor((double)N *5/16);
    int max_y_location = floor((double)N / 4);
    int min_z_location = ceil((double)N / 6);
    int max_z_location = floor((double)N / 2);


    
    
    // Create the first 3D matrix, all values are initialized to zero
    for (int i = 0; i < N;i++){
        for (int j = 0; j < N; j++){
            for (int k = 0; k < N; k++){           
                 if(i <= max_x_location && j <= max_y_location && k <= max_z_location && k >= min_z_location){
                    f[i][j][k] = 200; // Set the heater values
                }else{
                    f[i][j][k] = 0; 
                }
            }
        }
    }
    

    // Initialize the boundary values for the output, these will not be overwritten in the subsequent loops 
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            output[0][i][j] = 20;
            output[N-1][i][j] = 20;
            output[i][N-1][j] = 20;
            output[i][j][0] = 20;
            output[i][j][N-1] = 20;
            output[i][0][j] = 0;
        }
    }
    // boundary (wall)  conditions for input as well 
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                input[i][j][k] = output[i][j][k];
            }
        }
    }

    // Loop through Steps
    int steps = 0;
    while ( steps < iter_max && dif > tolerance){
        dif = 0;
         for (int i = 1; i < (N-1);i++){
            for (int j = 1; j < (N-1); j++){
                for (int k = 1; k < (N-1); k++){
                // Calculate new value - rewritten for readability.
                    
                    output[i][j][k] = (input[i-1][j][k] + 
                                    input[i+1][j][k] +
                                    input[i][j-1][k] +
                                    input[i][j+1][k] + 
                                    input[i][j][k-1] +
                                    input[i][j][k+1] +
                                    pow(delta,2) * f[i][j][k])/ 6.0;
                                                    
                    // Add norm
                    dif += pow(output[i][j][k] - input[i][j][k], 2);
                }
            }
        }
        // Calculate the difference criteria
        dif = sqrt(dif);
        printf("dif %lf\n", dif);
        //abs(input[0][0][0] - output[0][0][0]);
        // Make new input

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                for (int k = 0; k < N; k++) {
                    input[i][j][k] = output[i][j][k];
                }
            }
        }
        steps++;
    }
    return input;
}
