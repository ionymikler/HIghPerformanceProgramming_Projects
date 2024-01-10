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
    int wallrange[6];
    // TODO: Make the 8 cube corners have a very very large value for sanity check
    /* IMPORTANT: For ease of thinking, the range of the cube with side length 1 is [0,N-1]
    / So, origin is 0,0,0 and top, right-most, 
    */
}
