#!/bin/bash
declare -a OPT=(-O0 -O1 -O2 -O3)
declare -a N=(3 7 10 14 17 20 24 27)
declare -a run=(1 2 3 4 5 6 7 8 9 10)
for j in "${N[@]}"
  do
    for i in "${OPT[@]}"
      do
         echo "Optimization Level:$i N:$j M:$((4*$j))">> output.dat
         echo "Optimization Level:$i N:$j M:$((4*$j))">> outputclippedtime.dat
	 echo "Optimization Level:$i N:$j M:$((4*$j))">> outputclippedGUPS.dat
	 for pq in "${run[@]}"
	 do
	    make OPT=$i
	    ./gups_vanilla $j $((4*$j)) 1024 > output.dat
	    awk '$1=="Bad" {k=1;next};k && k++<=2' output.dat>>outputclippedtime.dat
	    awk '$1=="Update" {k=1;next};k && k++<=2' output.dat>>outputclippedGUPS.dat
	    make clean
	  done
      done
  done

