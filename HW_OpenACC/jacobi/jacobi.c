#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define OFFSET(x, y, m) (((x)*(m)) + (y))

void initialize(double *restrict A, double *restrict Anew, int m, int n)
{
    memset(A, 0, n * m * sizeof(double));
    memset(Anew, 0, n * m * sizeof(double));

    for(int i = 0; i < m; i++){
        A[i] = 1.0;
        Anew[i] = 1.0;
    }
}

void deallocate(double *restrict A, double *restrict Anew)
{
    free(A);
    free(Anew);
}

int main(int argc, char** argv)
{
    const int n = SIZE;
    const int m = SIZE;
    const int iter_max = MAXITER;

    const double tol = 1.0e-6;
    double error = 1.0;

    double *restrict A    = (double*)malloc(sizeof(double)*n*m);
    double *restrict Anew = (double*)malloc(sizeof(double)*n*m);

    initialize(A, Anew, m, n);

    printf("Jacobi relaxation Calculation: %d x %d mesh\n", n, m);

    double st = omp_get_wtime();
    int iter = 0;
    //#pragma acc data copyin(A[0:m*n]) copyout(Anew[0:m*n]) //Uncomment to move data from memory to GPU
    while ( error > tol && iter < iter_max )
    {
      double error = 0.0;
     
     // kernels directive implicitly takes care of parallelism of the loops that follow so it doesnt 
     // effect having the explicit loop parallelization

     //#pragma acc kernels //Uncomment to use kernels
     //{
      //#pragma acc parallel loop reduction(max:error) //Uncomment to use reduction
      for( int j = 1; j < n-1; j++)
      {
       //#pragma acc loop reduction(max:error) //Uncomment to use reduction
       for( int i = 1; i < m-1; i++ )
       {
        Anew[OFFSET(j, i, m)] = 0.25 * ( A[OFFSET(j, i+1, m)] + A[OFFSET(j, i-1, m)]
                                           + A[OFFSET(j-1, i, m)] + A[OFFSET(j+1, i, m)]);
	      error = fmax( error, fabs(Anew[OFFSET(j, i, m)] - A[OFFSET(j, i , m)]));
       }
      }
      //#pragma acc parallel loop
      for( int j = 1; j < n-1; j++)
      {
       //#pragma acc loop
       for( int i = 1; i < m-1; i++ )
        {
            A[OFFSET(j, i, m)] = Anew[OFFSET(j, i, m)];
        }
      }
     //} //kernel end
     if(iter % 100 == 0) printf("%5d, %0.6f\n", iter, error);
     iter++;
    }
    double runtime = omp_get_wtime() - st;

    printf("ExecutionTime: %f s\n", runtime);

    deallocate(A, Anew);

    return 0;
}

