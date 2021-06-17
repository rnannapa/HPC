gcc -I/opt/packages/papi/5.4.3/include/ -O0 matmul.c -L/opt/packages/papi/5.4.3/lib/ -lpapi -o matmul
module load papi
srun ./matmul
