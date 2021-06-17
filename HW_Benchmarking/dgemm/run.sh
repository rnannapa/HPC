#!/bin/bash
declare -a N=(10 40 160 640 2560 10240)
for j in "${N[@]}"
  do
    echo "N:$j">>output.dat
	./dgemm $j $j $j>>output.dat
  done
