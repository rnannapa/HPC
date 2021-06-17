#include <sys/time.h>
#include <stdio.h>

#ifndef SIZE
#define SIZE 2048
#endif

double a[SIZE][SIZE];
double b[SIZE][SIZE];
double c[SIZE][SIZE];
double d[SIZE][SIZE];

int main()
{
  int i,j,k;
  struct timeval tim;
  double t1, t2;
  double tmp;
  
  // Initialize matrices.
  for (i = 0; i < SIZE; ++i) {
    for (j = 0; j < SIZE; ++j) {
      a[i][j] = (double)(i + j);
      b[i][j] = (double)(i - j);
      c[i][j] = 0.0f;
      d[i][j] = 0.0f;
    }
  }

  // Time stamp t1

 #ifdef CASE0
  // Given Code
  // Compute matrix multiplication.
  gettimeofday(&tim, NULL);
  t1=tim.tv_sec+(tim.tv_usec/1000000.0);  
  for (i = 0; i < SIZE; ++i) {
    for (j = 0; j < SIZE; ++j) {
      for (k = 0; k < SIZE; ++k) {
         c[i][j] = a[i][k] * b[k][j] + c[i][j];
      }
    }
  }
  gettimeofday(&tim, NULL);
  t2=tim.tv_sec+(tim.tv_usec/1000000.0);
  printf("total: %.6lf\n", t2-t1);
#endif

#ifdef CASEnoK
printf("\n===============\n");
printf("\n Modified Code \n");
printf("\n===============\n");
  // Compute matrix multiplication.
  //Moving "a","b","c" from main memory to GPU and get back "c"
  gettimeofday(&tim, NULL);
  t1=tim.tv_sec+(tim.tv_usec/1000000.0);  
  #pragma acc data copyin(a,b) copy(c)   // Uncomment to move data from host to device
  #pragma acc parallel loop tile(32,32)  
  for (i = 0; i < SIZE; ++i) {
    for (j = 0; j < SIZE; ++j) {
      tmp=0.0f; // Uncomment to use reduction
      #pragma acc loop reduction(+:tmp)
      for (k = 0; k < SIZE; ++k) {
         tmp += a[i][k] * b[k][j]; // Uncomment to use reduction
         //c[i][j] = a[i][k] * b[k][j] + c[i][j]; //Comment to use reduction
      }
      c[i][j] = tmp; // Uncomment to use reduction
    }
  }
  gettimeofday(&tim, NULL);
  t2=tim.tv_sec+(tim.tv_usec/1000000.0);
  printf("total: %.6lf\n", t2-t1);
#endif
#ifdef CASEK
printf("\n===========================\n");
printf("\n Using kernal directive\n");
printf("\n===========================\n");
// Modified Code 2 //
  // Compute matrix multiplication.
  gettimeofday(&tim, NULL);
  t1=tim.tv_sec+(tim.tv_usec/1000000.0);  
  #pragma acc data copyin(a,b) copy(c) // Moving "a","b","c" from main memory to GPU and get back "c"//
//  #pragma acc kernels// Implicit optimization
  for (i = 0; i < SIZE; ++i) {
    for (j = 0; j < SIZE; ++j) {
      //tmp=0.0f;// Uncomment to use reduction
//      #pragma acc loop reduction(+:tmp)
      for (k = 0; k < SIZE; ++k) {
         c[i][j] = a[i][k] * b[k][j] + c[i][j];// Comment to use reduction
         //tmp += a[i][k] * b[k][j];// Uncomment to use reduction
      }
      //c[i][j] = tmp; // Uncomment to use reduction
    }
  }
  gettimeofday(&tim, NULL);
  t2=tim.tv_sec+(tim.tv_usec/1000000.0);
  printf("total: %.6lf\n", t2-t1);
#endif
  // Time stamp t2, elapsed time OpenACC
  //printf("%f FLOPS/sec \n", (SIZE^3)/(t2-t1));
#ifdef CHECK
  // ****************
  // double-check the OpenACC result 
  // ****************
  
  // Perform the multiplication
#pragma omp parallel for default(none) shared(a,b,d) private(i,j,k)
  for (i = 0; i < SIZE; ++i) 
    for (j = 0; j < SIZE; ++j) 
      for (k = 0; k < SIZE; ++k) 
	d[i][j] += a[i][k] * b[k][j];
 
  // Check the OpenACC result matrix
  for (i = 0; i < SIZE; ++i)
    for (j = 0; j < SIZE; ++j)
      if(c[i][j] != d[i][j]) {
	printf("Error %d %d %f %f \n", i,j, c[i][j], d[i][j]);
	exit(1);
      }
  printf("OpenACC matrix multiplication test was successful!\n");
#endif
  
  return 0;
}
