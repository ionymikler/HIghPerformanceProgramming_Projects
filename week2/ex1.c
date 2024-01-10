#include <stdio.h>
#include <cblas.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv){
    int n , i;
    double sum;
    n = atoi(argv[1]);
    printf("Value of N = %d\n", n);
    for (i = 1; i <= n; i++) {
        sum += 4 / (1 + pow((i - 0.5) / n, 2));
    }
    sum = sum / n;
    printf("Value of sum = %lf\n", sum);
}