#!/bin/bash
declare -a SIZE=(2048 4096 8192)
#declare -a SIZE=(8192)
:<<END_COMMENT
for j in "${SIZE[@]}"
 do
   echo "SIZE = $j"
   pgcc -acc -fast -ta=tesla:cc35 matrixMul.c -o matrixMul.out -DSIZE=$j -DCASEnoK
   ./matrixMul.out
   #nvprof --cpu-profiling on --metrics dram_read_transactions ./matrixMul.out
   #nvprof --system-profiling on ./matrixMul.out
 done
#rm *.out
END_COMMENT

for j in "${SIZE[@]}"
 do
   pgcc -acc -fast -ta=tesla:cc60 -Minfo=accel matrixMul.c -o matrixMul.out -DSIZE=$j -DCASEK   
   echo "SIZE $j" >> output"$j".dat
   nvprof --cpu-profiling on --metrics flop_count_dp  --metrics dram_read_transactions --metrics dram_write_transactions ./matrixMul.out 2>&1|tee cpu.log
   nvprof --system-profiling on ./matrixMul.out 2>&1|tee system.log
   awk '$1=="total:" {print "Time",$2}' system.log>>output"$j".dat
   awk '$1=="Power" {print "Power",$4;exit}' system.log>>output"$j".dat
   awk '$7 ~ /CUDA/ {print "Host_to_Device(us)",substr($1, 1, length($1)-1)}' system.log|sed -n '1p'>>output"$j".dat
   awk '$7 ~ /CUDA/ {print "Device_to_Host(us)",substr($1, 1, length($1)-1)}' system.log|sed -n '2p'>>output"$j".dat
   awk '$7 ~ /main/ {print $7,substr($1, 1, length($1)-1)}' system.log>>output"$j".dat
   awk '$1=="Device" {k=1;next};k && k++<=4' cpu.log|awk '{print $2,$9}'>>output"$j".dat
   awk '{print $1}' output"$j".dat > command.dat 
   awk '{print $2}' output"$j".dat > crop"$j".dat 
   rm *.out
done
paste -d ',' command.dat crop*.dat>final.dat
rm output*.dat crop*.dat
