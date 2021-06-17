#!/bin/bash
declare -a SIZE=(S M L)
for j in "${SIZE[@]}"
 do
echo "SIZE $j" >> output"$j".dat
   pgcc -acc -fast -ta=tesla:cc60 -Minfo=accel himenoBMT.c -o himenoBMT.out -D$j -DNN=20
   nvprof --system-profiling on ./himenoBMT.out 2>&1|tee system.log
   nvprof --cpu-profiling on --metrics flop_count_dp  --metrics dram_read_transactions --metrics dram_write_transactions ./himenoBMT.out 2>&1|tee cpu.log
   awk '$1=="cpu" {print "Time",$3}' system.log>>output"$j".dat
   awk '$1=="MFLOPS" {print "MFLOPS",$4}' system.log>>output"$j".dat
   awk '$1=="Power" {print "Power",$4;exit}' system.log>>output"$j".dat
   awk '$7 ~ /CUDA/ {print "Host_to_Device(us)",substr($1, 1, length($1)-1)}' system.log|sed -n '1p'>>output"$j".dat
   awk '$7 ~ /CUDA/ {print "Device_to_Host(us)",substr($1, 1, length($1)-1)}' system.log|sed -n '2p'>>output"$j".dat
   awk '$7 ~ /jacobi/ {print $7,substr($1, 1, length($1)-1)}' system.log>>output"$j".dat
   awk '$1=="Device" {k=1;next};k && k++<=12' cpu.log|awk '{print $2,$9}'>>output"$j".dat
   awk '{print $1}' output"$j".dat > command.dat 
   awk '{print $2}' output"$j".dat > crop"$j".dat
   rm *.out 
done
paste -d ',' command.dat crop*.dat>final.dat
rm output*.dat crop*.dat