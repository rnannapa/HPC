#include<stdio.h>
#include<stdlib.h>

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
  init_array();
  mat_vec();
  return 0;
}
