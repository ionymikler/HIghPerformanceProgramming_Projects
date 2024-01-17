void init_C(int m,int n,double **C);
void matmult_mkn_omp(int m,int n,int k,double **A,double **B,double **C);
void matmult_lib(int m, int n, int k, double **A, double **B, double **C);