#!/bin/bash
declare -a SIZE=(5000 25000 125000 625000 3125000 15625000 78125000)
#declare -a SIZE=(50 100 1000 5000 10000)
declare -a OPTS=(L1 L2 L3)

for j in "${SIZE[@]}"
  do
   echo "Size=$j">>output.dat
   for l in "${OPTS[@]}"
    do
     make SIZE=$j OPTS=$l
     module load papi
     ./outquicksort>>output.dat
     make clean
   done
  done
