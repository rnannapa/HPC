#!/bin/bash
declare -a OPT=(-O0 -O1 -O2 -O3)
declare -a SIZE=(100 1000 10000 1000000 10000000 10000000)
for j in "${SIZE[@]}"
  do
    for i in "${OPT[@]}"
      do	
        echo "Opt:$i Size:$j">>output.dat
	echo "Opt:$i Size:$j">>outputclipped.dat
        echo "Opt:$i"
        make OPT=$i SIZE=$j
        ./stream > output.dat
        awk '$1=="Function" {k=1;next};k && k++<=5' output.dat>>outputclipped.dat
	make clean
      done
  done
