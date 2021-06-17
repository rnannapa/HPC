#!/bin/bash
##SBATCH -N 2
##SBATCH --ntasks-per-node 1
##SBATCH -t 01:00:00
##SBATCH -o output_2N
#declare -a NPROCS=(4 8 16 32 56)
declare -a NPROCS=(32)
for n in "${NPROCS[@]}"
 do 
   module load mpi/gcc_openmpi
   echo "Run for $n procs"
#   echo "NPROC $n">>out"$n".dat
   mpicc exercise3.c -o out"$n".out -DNPROCS=$n
   mpirun -np $n out"$n".out
# >> out"$n".dat
   rm *.out 
 done
