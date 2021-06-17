#define min(x,y) (((x) < (y)) ? (x) : (y))

#include <stdio.h>
#include <stdlib.h>
#include "mkl.h"
#include "papi.h"

#define NUM_EVENTS 11
int main(int argc,char *argv[])
{
    double *A, *B, *C;
    int m, n, k, i, j;
    double alpha, beta;
    long long start_time, end_time;

    long long values[NUM_EVENTS];
    int eventset = PAPI_NULL;
    PAPI_library_init(PAPI_VER_CURRENT);
    PAPI_create_eventset(&eventset);
    printf ("\n This example computes real matrix C=alpha*A*B+beta*C using \n"
            " Intel(R) MKL function dgemm, where A, B, and  C are matrices and \n"
            " alpha and beta are double precision scalars\n\n");
    m = mdim; //atoi(argv[1]);
    k = kdim; //atoi(argv[2]);
    n = ndim; //atoi(argv[3]);
    /*m = 10480, k = 10480, n = 10480;*/
    printf (" Initializing data for matrix multiplication C=A*B for matrix \n"
            " A(%ix%i) and matrix B(%ix%i)\n\n", m, k, k, n);
    alpha = 1.0; beta = 0.0;

    printf (" Allocating memory for matrices aligned on 64-byte boundary for better \n"
            " performance \n\n");
    A = (double *)mkl_malloc( m*k*sizeof( double ), 64 );
    B = (double *)mkl_malloc( k*n*sizeof( double ), 64 );
    C = (double *)mkl_malloc( m*n*sizeof( double ), 64 );
    if (A == NULL || B == NULL || C == NULL) {
      printf( "\n ERROR: Can't allocate memory for matrices. Aborting... \n\n");
      mkl_free(A);
      mkl_free(B);
      mkl_free(C);
      return 1;
    }

    printf (" Intializing matrix data \n\n");
    for (i = 0; i < (m*k); i++) {
        A[i] = (double)(i+1);
    }

    for (i = 0; i < (k*n); i++) {
        B[i] = (double)(-i-1);
    }

    for (i = 0; i < (m*n); i++) {
        C[i] = 0.0;
    }

    #ifdef L1  
    PAPI_add_event(eventset, PAPI_L1_LDM);
    PAPI_add_event(eventset, PAPI_L1_STM);
    PAPI_add_event(eventset, PAPI_L1_ICM);
    PAPI_add_event(eventset, PAPI_L1_DCM);
    PAPI_add_event(eventset, PAPI_L1_TCM);
    #endif
    #ifdef L2
    PAPI_add_event(eventset, PAPI_L2_LDM);
    PAPI_add_event(eventset, PAPI_L2_STM);
    PAPI_add_event(eventset, PAPI_L2_ICM);
    PAPI_add_event(eventset, PAPI_L2_DCM);
    PAPI_add_event(eventset, PAPI_L2_TCM);
    #endif
    #ifdef L3M
    PAPI_add_event(eventset, PAPI_L3_LDM);
    PAPI_add_event(eventset, PAPI_L3_TCM);
    #endif
    #ifdef L3A
    PAPI_add_event(eventset, PAPI_L3_DCA);
    PAPI_add_event(eventset, PAPI_L3_DCR);
    PAPI_add_event(eventset, PAPI_L3_DCW);
    #endif 
    #ifdef FP  
    PAPI_add_event(eventset, PAPI_DP_OPS);
    //PAPI_add_event(eventset, PAPI_REF_CYC);
    //PAPI_add_event(eventset, PAPI_L1_TCR);
    #endif

    printf (" Computing matrix product using Intel(R) MKL dgemm function via CBLAS interface \n\n");
    PAPI_start(eventset);

    start_time=PAPI_get_real_usec();
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                m, n, k, alpha, A, k, B, n, beta, C, n);
    end_time=PAPI_get_real_usec();

    PAPI_read(eventset, values);
    PAPI_stop(eventset, values);
    printf("PAPI_START\n");
    #ifdef L1 
    printf("L1 Load Misses = %lli\n",values[0]);
    printf("L1 Store Misses = %lli\n",values[1]);
    printf("L1 Instruction Cache Misses = %lli\n",values[2]);
    printf("L1 Data Cache Misses = %lli\n",values[3]);
    printf("L1 Total Cache Misses = %lli\n",values[4]);
    #endif
    #ifdef L2
    printf("L2 Load Misses = %lli\n",values[0]);
    printf("L2 Store Misses = %lli\n",values[1]);
    printf("L2 Instruction Cache Misses = %lli\n",values[2]);
    printf("L2 Data Cache Misses = %lli\n",values[3]);
    printf("L2 Total Cache Misses = %lli\n",values[4]);
    #endif
    #ifdef L3M  
    printf("L3 Load Misses = %lli\n",values[0]);
    printf("L3 Total Cache Misses = %lli\n",values[1]);
    printf("Time= %lli\n",end_time-start_time);
    #endif
    #ifdef L3A  
    printf("L3 Data Cache Access = %lli\n",values[0]);
    printf("L3 Data Cache Read = %lli\n",values[1]);
    printf("L3 Data Cache Write = %lli\n",values[2]);
    #endif
    #ifdef FP  
    printf("Floating Point operations = %lli\n",values[0]);
    #endif
    
    printf("Time= %lli\n",end_time-start_time);
    printf("PAPI_FINISH\n");
    PAPI_reset(eventset);
    
    printf ("\n Computations completed.\n\n");

    printf (" Top left corner of matrix A: \n");
    for (i=0; i<min(m,6); i++) {
      for (j=0; j<min(k,6); j++) {
        printf ("%12.0f", A[j+i*k]);
      }
      printf ("\n");
    }

    printf ("\n Top left corner of matrix B: \n");
    for (i=0; i<min(k,6); i++) {
      for (j=0; j<min(n,6); j++) {
        printf ("%12.0f", B[j+i*n]);
      }
      printf ("\n");
    }
    
    printf ("\n Top left corner of matrix C: \n");
    for (i=0; i<min(m,6); i++) {
      for (j=0; j<min(n,6); j++) {
        printf ("%12.5G", C[j+i*n]);
      }
      printf ("\n");
    }

    printf ("\n Deallocating memory \n\n");
    mkl_free(A);
    mkl_free(B);
    mkl_free(C);

    printf (" Example completed. \n\n");
    return 0;
}
