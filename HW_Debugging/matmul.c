#include <stdio.h>
#include <stdlib.h>
#include "papi.h"
#define N 1000
#define NUM_EVENTS 11
#define CB1L1
//#define CB1L2
//#define CB2L2
//#define CB2L3
//#define TLB
//#define M
//#define IE
//#define BR
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

void classic_matmul()
{
        // Multiply the two matrices
        int i, j, k;
        for (i = 0; i < N; i++) 
	{
	    for (j = 0; j < N; j++) 
	    {
                float sum = 0.0;
                for (k = 0; k < N; k++) 
		{
                 sum += matrix_a[i][k] * matrix_b[k][j];
                }
             matrix_c[i][j] = sum;
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

	/* Cache behavior 1*/
	#ifdef CB1L1
	PAPI_add_event(eventset, PAPI_L1_ICM);
	PAPI_add_event(eventset, PAPI_L1_DCM);
	PAPI_add_event(eventset, PAPI_L1_TCM);
	PAPI_add_event(eventset, PAPI_L1_LDM);
	PAPI_add_event(eventset, PAPI_L1_STM);
	#endif
	#ifdef CB1L2
	PAPI_add_event(eventset, PAPI_L2_ICM);
	PAPI_add_event(eventset, PAPI_L2_DCM);
	PAPI_add_event(eventset, PAPI_L2_TCM);
        PAPI_add_event(eventset, PAPI_L2_LDM);
        PAPI_add_event(eventset, PAPI_L2_STM);
	#endif

	/* Cache behavior 2*/
	#ifdef CB2L2
      	PAPI_add_event(eventset, PAPI_L2_ICA);
        PAPI_add_event(eventset, PAPI_L2_DCA);
        PAPI_add_event(eventset, PAPI_L2_DCR);
        PAPI_add_event(eventset, PAPI_L2_ICR);
        PAPI_add_event(eventset, PAPI_L2_DCW);
        PAPI_add_event(eventset, PAPI_L2_TCW);
        #endif
	#ifdef CB2L3
	PAPI_add_event(eventset, PAPI_L3_ICA);
        PAPI_add_event(eventset, PAPI_L3_DCA);
        PAPI_add_event(eventset, PAPI_L3_DCR);
        PAPI_add_event(eventset, PAPI_L3_ICR);
        PAPI_add_event(eventset, PAPI_L3_DCW);
	#endif

	/* TLB */       
	#ifdef TLB
	PAPI_add_event(eventset, PAPI_TLB_DM);
        PAPI_add_event(eventset, PAPI_TLB_IM);
        #endif

	/* Memory */
	#ifdef M
	PAPI_add_event(eventset, PAPI_LD_INS);
        PAPI_add_event(eventset, PAPI_SR_INS);
	PAPI_add_event(eventset, PAPI_MEM_WCY);
	#endif        
	
	#ifdef IE
	PAPI_add_event(eventset, PAPI_TOT_INS);
        PAPI_add_event(eventset, PAPI_LST_INS);
	PAPI_add_event(eventset, PAPI_STL_ICY);
        PAPI_add_event(eventset, PAPI_FUL_ICY);
        PAPI_add_event(eventset, PAPI_STL_CCY);
        PAPI_add_event(eventset, PAPI_FUL_CCY);
	#endif

	/* BRANCHING */
	#ifdef BR
	PAPI_add_event(eventset, PAPI_BR_INS);
        PAPI_add_event(eventset, PAPI_BR_MSP);
        PAPI_add_event(eventset, PAPI_BR_TKN);
        PAPI_add_event(eventset, PAPI_L3_TCM);
	PAPI_add_event(eventset, PAPI_RES_STL);
	PAPI_add_event(eventset, PAPI_TOT_CYC);
	PAPI_add_event(eventset, PAPI_L3_LDM);
	#endif

	PAPI_start(eventset);
  	start_time = PAPI_get_real_usec();
  	classic_matmul();
  	end_time = PAPI_get_real_usec();
      //  reset_matrix();
	PAPI_read(eventset, values);
        PAPI_stop(eventset, values);

        printf("Done with simple maltiply\n");
	/*Cache Behavior 1*/
	#ifdef CB1L1	
	printf("L1 Instruction cache misses = %lli\n", values[0]);
        printf("L1 Data cache misses = %lli\n", values[1]);
        printf("L1 Total cache misses = %lli\n", values[2]);
        printf("L1 Load misses = %lli\n", values[3]);
        printf("L1 Store misses = %lli\n", values[4]);
        #endif

	#ifdef CB1L2
	printf("L2 Instruction cache misses = %lli\n", values[0]);
        printf("L2 Data cache misses = %lli\n", values[1]);
        printf("L2 Total cache misses = %lli\n", values[2]);
        printf("L2 Load misses = %lli\n", values[3]);
        printf("L2 Store misses = %lli\n", values[4]);
	printf("execution time = %lli\n ", end_time - start_time);
	#endif

	/*Cache Behavior 2*/
	#ifdef CB2L2
        printf("L2 Instruction cache access = %lli\n", values[0]);
        printf("L2 Data cache access = %lli\n", values[1]);
        printf("L2 Data cache read = %lli\n", values[2]);
        printf("L2 Instruction cache read= %lli\n", values[3]);
        printf("L2 Data cache write = %lli\n", values[4]);
        printf("L2 Total cache write = %lli\n", values[5]);
        #endif

	#ifdef CB2L3
	printf("L3 Instruction cache access = %lli\n", values[0]);
        printf("L3 Data cache access = %lli\n", values[1]);
        printf("L3 Data cache read = %lli\n", values[2]);
        printf("L3 Instruction cache read = %lli\n", values[3]);
        printf("L3 Data cache write = %lli\n", values[4]);
	#endif

	/*TLB and Memory */
	#ifdef TLB
	printf("Data TLB misses = %lli\n", values[0]);
        printf("Instruction TLB misses = %lli\n", values[1]);
	#endif
	
	#ifdef M
	printf("Load instructions = %lli\n", values[0]);
        printf("Store instructions = %lli\n", values[1]);
        printf("Cycles stalled waiting for memory writes = %lli\n", values[2]);
	#endif

	#ifdef IE
	printf("Instructions completed = %lli\n", values[0]);
	printf("load/store instructions completed = %lli\n", values[1]);
	printf("Cycles with no instruction issues = %lli\n", values[2]);
        printf("Cycles with maximum instruction issues = %lli\n", values[3]);
        printf("Cycles with no instructions completed = %lli\n", values[4]);
        printf("Cycles with maximum instructions completed = %lli\n", values[5]);
	#endif
        
	#ifdef BR
	printf("Branch Instructions = %lli\n", values[0]);
        printf("Conditional branch instructions mispredicted = %lli\n", values[1]);
        printf("Conditional branch instructions taken = %lli\n", values[2]);
        printf("L3 Total Cache misses= %lli\n", values[3]);
        printf("Cycles stalled on any resource = %lli\n", values[4]);
	printf("Total Cycles = %lli\n",values[5]);
	printf("L3 Load misses = %lli\n", values[6]);
	#endif	

	PAPI_reset(eventset);

	PAPI_start(eventset);
        st = PAPI_get_real_usec();
        interchanged_matmul();
        et = PAPI_get_real_usec();
        PAPI_read(eventset, values);
        PAPI_stop(eventset, values);

        printf("\nDone with interchanged multiply\n");
	/*Cache Behavior 1*/
	
	#ifdef CB1L1
	printf("L1 Instruction cache misses = %lli\n", values[0]);
        printf("L1 Data cache misses = %lli\n", values[1]);
        printf("L1 Total cache misses = %lli\n", values[2]);
        printf("L1 Load misses = %lli\n", values[3]);
        printf("L1 Store misses = %lli\n", values[4]);
	#endif

        #ifdef CB1L2
        printf("L2 Instruction cache misses = %lli\n", values[0]);
        printf("L2 Data cache misses = %lli\n", values[1]);
        printf("L2 Total cache misses = %lli\n", values[2]);
        printf("L2 Load misses = %lli\n", values[3]);
        printf("L2 Store misses = %lli\n", values[4]);
	printf("execution time = %lli\n ", et - st);
	#endif

	/*Cache Behavior 2*/
	#ifdef CB2L2
        printf("L2 Instruction cache access = %lli\n", values[0]);
        printf("L2 Data cache access = %lli\n", values[1]);
        printf("L2 Data cache read = %lli\n", values[2]);
        printf("L2 Instruction cache read= %lli\n", values[3]);
        printf("L2 Data cache write = %lli\n", values[4]);
        printf("L2 Total cache write = %lli\n", values[5]);
	#endif

        #ifdef CB2L3
        printf("L3 Instruction cache access = %lli\n", values[0]);
        printf("L3 Data cache access = %lli\n", values[1]);
        printf("L3 Data cache read = %lli\n", values[2]);
        printf("L3 Instruction cache read = %lli\n", values[3]);
        printf("L3 Data cache write = %lli\n", values[4]);
	#endif

	/*TLB and Memory */
	#ifdef TLB
	printf("Data TLB misses = %lli\n", values[0]);
        printf("Instruction TLB misses = %lli\n", values[1]);
        #endif

	#ifdef M
	printf("Load instructions = %lli\n", values[0]);
        printf("Store instructions = %lli\n", values[1]);
        printf("Cycles stalled waiting for memory writes = %lli\n", values[2]);
        #endif

	#ifdef IE
	printf("Instructions completed = %lli\n", values[0]);
	printf("load/store instructions completed = %lli\n", values[1]);
	printf("Cycles with no instruction issues = %lli\n", values[2]);
        printf("Cycles with maximum instruction issues = %lli\n", values[3]);
        printf("Cycles with no instructions completed = %lli\n", values[4]);
        printf("Cycles with maximum instructions completed = %lli\n", values[5]);
	#endif
        
	#ifdef BR
	printf("Branch Instructions = %lli\n", values[0]);
        printf("Conditional branch instructions mispredicted = %lli\n", values[1]);
        printf("Conditional branch instructions taken = %lli\n", values[2]);
        printf("L3 Total Cache misses= %lli\n", values[3]);
        printf("Cycles stalled on any resource = %lli\n", values[4]);
	printf("Total Cycles = %lli\n",values[5]);
	printf("L3 Load misses = %lli\n", values[6]);
	#endif

	PAPI_reset(eventset);

}
