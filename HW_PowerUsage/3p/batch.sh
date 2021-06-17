#!/bin/bash
declare -a SIZE=(5000 25000 125000 625000 3125000 15625000 78125000)
#declare -a SIZE=(50 100 1000 5000 10000)
declare -a OPTS=(L1 L2 L3M L3A)
#declare -a OPTS=(L3A)
#declare -a OPTS=(IC)
gcc -O2 -Wall -o raplread rapl_read.c -lm -s
for j in "${SIZE[@]}"
  do
   echo "Size=$j">>outputPAPI.dat
   echo "Size=$j">>outputPower.dat
   echo "Size=$j">>clipped.dat
   for l in "${OPTS[@]}"
    do
     make quicksortPAPI SIZE=$j OPTS=$l
     module load papi
     ./quicksortPAPI>>outputPAPI.dat
     make clean
    done
   make quicksortPower SIZE=$j
   ./raplread>outputPower.dat
   awk '/^Executing$/{flag=1; next} /^Size$/{flag=0} flag' outputPower.dat>>clipped.dat
   make clean
  done
