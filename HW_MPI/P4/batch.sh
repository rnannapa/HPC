#!/bin/bash
declare -a np=(2 4 8 10 12 16 20 24 28)
declare -a size=(512 1024 2048)
#declare -a np=(2 4 8)
#declare -a size=(10 20)
echo -e "1,2,4,8,10,12,16,20,24,28">>final.csv
for n in "${size[@]}"
do
echo "Size = $n"
  mpicc mm.c -DMATSIZE=$n -o mm.out
  gcc matmul.c -DN=$n -o matmul.out
  for i in "${np[@]}"
  do
    echo "Running $i processors"
    mpirun -np $i ./mm.out > re"$i".dat
  done
  ./matmul.out >> seq.dat
  paste -d ',' re2.dat re4.dat re8.dat re10.dat re12.dat re16.dat re20.dat re24.dat re28.dat>>result.dat
  rm *.out
done
paste -d ',' seq.dat result.dat>>final.csv
rm *.dat
