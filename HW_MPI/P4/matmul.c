#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
	clock_t t;
	t = clock();
        interchanged_matmul();
	t = clock() - t;
	double time_taken = ((double)t)/CLOCKS_PER_SEC;
	printf("%f\n",time_taken);
}
