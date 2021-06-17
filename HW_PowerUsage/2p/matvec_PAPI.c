#include<stdio.h>
#include<stdlib.h>
#include"papi.h"
#define NUM_EVENTS 11

float matrix_a[N][N];
float matrix_b[N];
float matrix_c[N];


void init_array()
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            matrix_a[i][j] = (1+(i*j)%1024)/2.0;
        }
        matrix_b[i] = (1+(i*j)%1024)/2.0;
    }
/*
    for (i=0;i<N;i++)
    {
    	for(j=0;j<N;j++)
    	{
    		printf("%f\t",matrix_a[i][j] );
        }
        printf("\n");
    }

    for (i=0;i<N;i++)
    {
    	printf("%f\n",matrix_b[i]);
    }
    printf("\n");
*/
}

void mat_vec()
{
  int i, j;
  for (i=0; i<N; i++)
   {
   for (j=0; j<N; j++)
    {
     matrix_c[i] += matrix_a[i][j]*matrix_b[j];
    }
   }
/*
  for (i=0;i<N;i++)
    {
    	printf("%f\n",matrix_c[i]);
    }
*/
}


int main()
{ 
  long long start_time, end_time;
  long long values[NUM_EVENTS];
  int eventset = PAPI_NULL;
  PAPI_library_init(PAPI_VER_CURRENT);
  PAPI_create_eventset(&eventset);
  init_array();

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
  PAPI_start(eventset);
  start_time=PAPI_get_real_usec();
  mat_vec();
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
 #ifdef L3A
  printf("L3 Load Misses = %lli\n",values[0]);
  printf("L3 Total Cache Misses = %lli\n",values[1]);
  printf("Time= %lli\n",end_time-start_time);
 #endif
 #ifdef L3A
  printf("L3 Cache Access = %lli\n",values[0]);
  printf("L3 Cache Read = %lli\n",values[1]);
  printf("L3 Cache Write = %lli\n",values[2]);
 #endif
  printf("PAPI_FINISH\n");
  PAPI_reset(eventset);
  
  return 0;
}
