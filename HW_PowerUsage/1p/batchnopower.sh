#!/bin/bash
#declare -a SIZE=(500 1000 1500 2000 2500)
declare -a SIZE=(2000)
#declare -a SIZE=(4000 3000 2500 2000 1500 1000 500)
#declare -a SIZE=(100)
#declare -a OPTS=(L1 L2 L3M L3A FP)
#gcc -O2 -Wall -o raplread rapl_read.c -lm -s
for j in "${SIZE[@]}"
  do
    echo "Size:$j"
    echo "Size=$j">>PAPI_OUT.dat
    echo "Size=$j">>output_mklmul_PAPI.dat
    echo "Size=$j">>output_matmul_PAPI.dat
    echo "mklmul Size=$j">>clipped_mklmul_PAPI.dat
    echo "matmul Size=$j">>clipped_matmul_PAPI.dat
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
  #   rm output*
  done
