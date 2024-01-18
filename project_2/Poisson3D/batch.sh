#!/bin/sh 
### General options 
### -- specify queue -- 
#BSUB -q hpc

### -- set the job Name --
#BSUB -J batchjob

### -- ask for number of cores (default: 1) -- 
#BSUB -n 1 
# does not work for 16 and 32

### -- specify that the cores must be on the same host -- 
#BSUB -R "span[hosts=1] affinity[socket(1)]"

### -- specify memory need per core/slot -- 
#BSUB -R "rusage[mem=1024]"

### -- set walltime limit: hh:mm -- 
#BSUB -W 20

### -- send notification at start -- 
#BSUB -B

### -- send notification at completion -- 
#BSUB -N

### -- Specify the output and error file. %J is the job-id -- 
### -- -o and -e mean append, -oo and -eo mean overwrite -- 
##BSUB -o Output_%J.out
##BSUB -e Output_%J.err 

Ns="10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95 100 150"

iterations=25000
tolerance=0.000001
startT=0
output_type=0

rm -f "results_gs.txt"
rm -f "results_j.txt"

for N in $Ns
do
    ./poisson_gs $N $iterations $tolerance $startT $output_type
    ./poisson_j $N $iterations $tolerance $startT $output_type
done


exit 0
