//Source code courtesy: internet

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "papi.h"
//#define N 268435456
#define NUM_EVENTS 11
float array_a[N];
void quicksort(float*, int, int);

void init_array()
{
    int i;
    time_t t;
    srand((unsigned) time(&t));

    for (i = 0; i < N; i++) {
        array_a[i] = rand()*i*1024*2.0;
    }
}


int main()
{
  
  init_array();
  long long values[NUM_EVENTS];
  int eventset = PAPI_NULL;
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
 #ifdef L3
  PAPI_add_event(eventset, PAPI_L3_LDM);
  PAPI_add_event(eventset, PAPI_L3_TCM);
 #endif

  PAPI_start(eventset);
  quicksort(array_a, 0, N-1);
  PAPI_read(eventset, values);
  PAPI_stop(eventset, values);
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
 #ifdef L3  
  printf("L3 Load Misses = %lli\n",values[0]);
  printf("L3 Total Cache Misses = %lli\n",values[1]);
 #endif  
  PAPI_reset(eventset);
// printf("Sorted array\n");  
// for(int i = 0; i < N; i++)
//   printf("%8.2f\n", array_a[i]);

  return 0;
}

void quicksort(float *arr, int low, int high)
{
  int pivot, i, j;
  float temp;

  if(low < high) {
    pivot = low; // select a pivot element
    i = low;
    j = high;
    while(i < j) {
      // increment i till you get a number greater than the pivot element
      while(arr[i] <= arr[pivot] && i <= high)
        i++;
      // decrement j till you get a number less than the pivot element
      while(arr[j] > arr[pivot] && j >= low)
        j--;
      // if i < j swap the elements in locations i and j
      if(i < j) {
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
      }
    }

    // when i >= j it means the j-th position is the correct position
    // of the pivot element, hence swap the pivot element with the
    // element in the j-th position
    temp = arr[j];
    arr[j] = arr[pivot];
    arr[pivot] = temp;
    // Repeat quicksort for the two sub-arrays, one to the left of j
    // and one to the right of j
    quicksort(arr, low, j-1);
    quicksort(arr, j+1, high);
  }
}
