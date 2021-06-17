#!/bin/bash
#SBATCH -N 56
#SBATCH --ntasks-per-node 1
#SBATCH -t 01:00:00
#SBATCH -o output_2N

declare -a N=(1 10 100 1000 10000 100000 1000000 10000000)
#declare -a N=(1000000 10000000)
for j in "${N[@]}"
 do
  echo "Blocking Size $j"
  mpicc ping_pong_blocking.c -o blocking"$j".out -DN=$j
  mpirun -np 56 --bind-to core --map-by node ./blocking"$j".out>blk.dat
  awk '{print $9}' blk.dat>blocking"$j".dat
  echo "Non Blocking Size $j"
  mpicc ping_pong_non_blocking.c -o non_blocking"$j".out -DN=$j
  mpirun -np 56 --bind-to core --map-by node ./non_blocking"$j".out>nblk.dat
  awk '{print $9}' nblk.dat>nonblocking"$j".dat
  rm *.out
done
awk '{print substr($6, 1, length($6)-1)}' blk.dat>dest.dat
paste -d ',' dest.dat blocking1.dat blocking10.dat blocking100.dat blocking1000.dat blocking10000.dat blocking100000.dat blocking1000000.dat blocking10000000.dat>finalBlocking.dat
paste -d ',' dest.dat nonblocking1.dat nonblocking10.dat nonblocking100.dat nonblocking1000.dat nonblocking10000.dat nonblocking100000.dat nonblocking1000000.dat nonblocking10000000.dat>finalNonBlocking.dat
rm blk.dat nblk.dat dest.dat blocking*.dat nonblocking*.dat
