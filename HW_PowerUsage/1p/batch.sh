#!/bin/bash
declare -a SIZE=(500 1000 1500 2000 2500)
#declare -a SIZE=(3000)
#4000 5000)
#declare -a SIZE=(4000 3000 2500 2000 1500 1000 500)
#declare -a SIZE=(100)
declare -a OPTS=(L1 L2 L3M L3A FP)
#gcc -O2 -Wall -o raplread rapl_read.c -lm -s
for j in "${SIZE[@]}"
  do
    echo "Size:$j"
    echo "Size=$j">>PAPI_OUT.dat
    echo "Size=$j">>POWER_OUT.dat
    echo "Size=$j">>output_mklmul_PAPI.dat
    echo "Size=$j">>output_matmul_PAPI.dat
    echo "Size=$j">>output_mklmul_Power.dat
    echo "Size=$j">>output_matmul_Power.dat
    echo "mklmul Size=$j">>clipped_mklmul_PAPI.dat
    echo "matmul Size=$j">>clipped_matmul_PAPI.dat
    echo "mklmul Size=$j">>clipped_mklmul_Power.dat
    echo "matmul Size=$j">>clipped_matmul_Power.dat
    for l in "${OPTS[@]}"
    do 
     make mklMul_PAPI matmul_PAPI SIZE=$j OPTS=$l 
     module load papi
     ./mklMul_PAPI.out>output_mklmul_PAPI.dat
     ./matmul_PAPI.out>output_matmul_PAPI.dat
     awk '/^PAPI_START$/{flag=1; next} /^PAPI_FINISH$/{flag=0} flag' output_mklmul_PAPI.dat>>clipped_mklmul_PAPI.dat
     awk '/^PAPI_START$/{flag=1; next} /^PAPI_FINISH$/{flag=0} flag' output_matmul_PAPI.dat>>clipped_matmul_PAPI.dat
     paste -d',' clipped_mklmul_PAPI.dat clipped_matmul_PAPI.dat > PAPI_OUT.dat
     make clean
    done
     make mklMul_Power matmul_Power SIZE=$j
     gcc -O2 -Wall -o raplread1.out rapl_read1.c -lm -s
     ./raplread1.out>output_mklmul_Power.dat
     awk '/^Executing$/{flag=1; next} /^Size$/{flag=0} flag' output_mklmul_Power.dat>>clipped_mklmul_Power.dat
     gcc -O2 -Wall -o raplread2.out rapl_read2.c -lm -s
     ./raplread2.out>output_matmul_Power.dat
     awk '/^Executing$/{flag=1; next} /^Size$/{flag=0} flag' output_matmul_Power.dat>>clipped_matmul_Power.dat
    # paste -d',' clipped_mklmul_Power.dat clipped_matmul_Power.dat > POWER_OUT.dat
     paste clipped_mklmul_Power.dat clipped_matmul_Power.dat > POWER_OUT.dat
     make clean
     rm output*
  done
