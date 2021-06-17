#!/bin/bash
#SBATCH -N 56
#SBATCH -n 56
#SBATCH --ntasks-per-node 1
#SBATCH -t 00:01:00
#SBATCH -o broad_red

declare -a SIZE=(1000000 10000000 100000000)
declare -a PROC=(2 4 8 16 32 56)
#declare -a SIZE=(1 10 100)
#declare -a PROC=(2 16 56)
for j in "${SIZE[@]}"
 do
   echo "running for $j"
   mpicc broadcast.c -o broad.out -DN=$j
   mpicc reduce.c -o red.out -DN=$j
   for p in "${PROC[@]}"
   do
     echo "Running Size $j for processes $p"
     echo "$j">pro"$j".dat
     echo "$p">>broadcast"$p".dat
     echo "$p">>reduce"$p".dat
     mpirun -np $p --bind-to core --map-by node ./broad.out>broad.dat
     awk '{print $1}' broad.dat>>pro"$j".dat
     awk '{print $3}' broad.dat>>broadcast"$p".dat
     mpirun -np $p --bind-to core --map-by node ./red.out>red.dat
     awk '{print $3}' red.dat>>reduce"$p".dat
   done
   paste -d ',' pro"$j".dat broadcast2.dat broadcast4.dat broadcast8.dat broadcast16.dat broadcast32.dat broadcast56.dat>Fbroadcast"$j".dat
   paste -d ',' pro"$j".dat reduce2.dat reduce4.dat reduce8.dat reduce16.dat reduce32.dat reduce56.dat>Freduce"$j".dat
   rm *.out pro*.dat broad*.dat red*.dat
done
cat Fbroadcast1000000.dat Fbroadcast10000000.dat Fbroadcast100000000.dat>finalBroadcast.dat
cat Freduce1000000.dat Freduce10000000.dat Freduce100000000.dat>finalReduce.dat

rm Fbroadcast*.dat Freduce*.dat




