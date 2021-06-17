#!/bin/bash
declare -a SIZE=(500 1000 1500 2000 2500 3000 4000 5000)
#declare -a SIZE=(500 1000) 
declare -a OPTS=(L1 L2 L3M L3A)
#gcc -O2 -Wall -o raplread rapl_read.c -lm -s
for j in "${SIZE[@]}"
  do
    echo "Size=$j"
    echo "Size=$j">>PAPI_OUT.dat
    echo "Size=$j">>POWER_OUT.dat
    echo "Size=$j">>output_mklmul_PAPI.dat
    echo "Size=$j">>output_matvec_PAPI.dat
    echo "Size=$j">>output_mklmul_Power.dat
    echo "Size=$j">>output_matvec_Power.dat
    echo "mklmul Size=$j">>clipped_mklmul_PAPI.dat
    echo "matmul Size=$j">>clipped_matvec_PAPI.dat
    echo "mklmul Size=$j">>clipped_mklmul_Power.dat
    echo "matmul Size=$j">>clipped_matvec_Power.dat
    for l in "${OPTS[@]}"
    do 
     make mklMul_PAPI matvec_PAPI SIZE=$j OPTS=$l 
     module load papi
     #./mklMul $j $j $j>outputPAPI.dat
     ./mklMul_PAPI.out>output_mklmul_PAPI.dat
     ./matvec_PAPI.out>output_matvec_PAPI.dat
     awk '/^PAPI_START$/{flag=1; next} /^PAPI_FINISH$/{flag=0} flag' output_mklmul_PAPI.dat>>clipped_mklmul_PAPI.dat
     awk '/^PAPI_START$/{flag=1; next} /^PAPI_FINISH$/{flag=0} flag' output_matvec_PAPI.dat>>clipped_matvec_PAPI.dat
     paste -d',' clipped_mklmul_PAPI.dat clipped_matvec_PAPI.dat > PAPI_OUT.dat
     make clean
    done
     make mklMul_Power matvec_Power SIZE=$j
     gcc -O2 -Wall -o raplread_1.out rapl_read1.c -lm -s
     ./raplread_1.out>output_mklmul_Power.dat
     awk '/^Executing$/{flag=1; next} /^Size$/{flag=0} flag' output_mklmul_Power.dat>>clipped_mklmul_Power.dat
     gcc -O2 -Wall -o raplread_2.out rapl_read2.c -lm -s
     ./raplread_2.out>output_matvec_Power.dat
     awk '/^Executing$/{flag=1; next} /^Size$/{flag=0} flag' output_matvec_Power.dat>>clipped_matvec_Power.dat
     paste clipped_mklmul_Power.dat clipped_matvec_Power.dat > POWER_OUT.dat
     make clean
     rm output*
  done
