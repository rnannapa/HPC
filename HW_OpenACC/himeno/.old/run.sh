#Compiling
pgcc -c -acc -fast -Minfo=accel himenoBMT.c -DSMALL
pgcc -acc -fast -ta=tesla:cc35 -Minfo=accel himenoBMT.o -o bmt

#Get GPU performance data
nvprof --system-profiling on ./bmt

#Get CPU performance data
nvprof --cpu-profiling on --metrics dram_read_transactions ./bmt

#Naive execution for testing outputs
#./bmt
