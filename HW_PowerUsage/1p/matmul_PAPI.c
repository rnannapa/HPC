#include <stdio.h>
#include <stdlib.h>
#include "papi.h"
#define NUM_EVENTS 11
float matrix_a[N][N];
float matrix_b[N][N];
float matrix_c[N][N];

void init_array()
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            matrix_a[i][j] = (1+(i*j)%1024)/2.0;
            matrix_b[i][j] = (1+(i*j)%1024)/2.0;
        }
    }
}

void interchanged_matmul()
{
        // Multiply the two matrices
        int i, j, k;
        for (i = 0; i < N; i++) 
	{
            for (k = 0; k < N; k++) 
	    {
                for (j = 0; j < N; j++) 
		{
                 matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
                }
            }
        }
}

int main(int argc, char **argv){
        int i,j;
        init_array();
	long long start_time, end_time, st, et;
  	long long values[NUM_EVENTS];
  	long long values1[NUM_EVENTS];
  	int eventset = PAPI_NULL;
  	int eventset1 = PAPI_NULL;
  	PAPI_library_init(PAPI_VER_CURRENT);
 	PAPI_create_eventset(&eventset);
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

	PAPI_start(eventset);
	start_time=PAPI_get_real_usec();
	interchanged_matmul();
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
        printf("Floating Point Operations = %lli\n",values[0]);
	#endif
        printf("Time= %lli\n",end_time-start_time);
	printf("PAPI_FINISH\n");
	PAPI_reset(eventset);

}
