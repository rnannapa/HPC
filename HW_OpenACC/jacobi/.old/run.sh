#Compiling
pgcc -acc -fast -ta=tesla:cc60 -Minfo=accel p1.c -o jacobi.out -DSIZE=1024

#Get GPU performance data
nvprof --system-profiling on ./jacobi.out

#Get CPU performance data
nvprof --cpu-profiling on --metrics dram_read_transactions ./jacobi.out

#Naive execution for testing outputs
#./jacobi
