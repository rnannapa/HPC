#include<stdio.h>
#include<stdlib.h>

float matrix_a[N][N];
float matrix_b[N];
float matrix_c[N];

void init_array()
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            matrix_a[i][j] = (1+(i*j)%1024)/2.0;
            printf("%f\t",matrix_a[i][j]);
        }
        printf("\n");
        
        matrix_b[i] = (1+(i*j)%1024)/2.0;
     	printf("%f\n",matrix_b[i] );   
    }
}

int main(int argc, char const *argv[])
{
	init_array();

	for ( int i = 0; i < N; i++ )
	{
		matrix_c[i] = 0;
		for ( int j = 0; j < N; j++ )
		{
			matrix_c[i] += matrix_a[i][j] * matrix_b[j];
		}
		printf("%f\n",matrix_c[i]);
	}
}
