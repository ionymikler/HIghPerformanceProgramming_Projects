#include <stdbool.h>

void populate_range(double ***p, int startsEnds[6], double val);
void populate_walls(double ***p, int N, double temp);
void init_cube(double ***p, int N, double start_T);
void init_force(double ***f, int N);
void sum_u(double ***u, int N);
double get_sum_u(double ***u, int N);
void print_params(int N, int iter_max, double tolerance, double start_T, int thread_num, bool verbose, int output_type);