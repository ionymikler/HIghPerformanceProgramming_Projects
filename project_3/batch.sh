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

Ns="10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95 100"

iterations=25000
tolerance=0.0
startT=0

rm -f "Poisson_map_baseline/results.txt"
rm -f "Poisson_map/results.txt"
rm -f "Poisson_map_ES/results.txt"

# Compare data transfering strategies
for N in $Ns
do
    ./Poisson_baseline/poisson $N $iterations $startT # cpu parralize
    ./Poisson_map/poisson $N $iterations $startT
    # ./Poisson_memcpy/poisson $N $iterations $startT
done

# running same results with stopping criteria
for N in $Ns
do
    # ./Poisson_barline_ES/poisson $N $iterations $startT $tolerance
    ./Poisson_map_ES/poisson $N $iterations $startT $tolerance
    # ./Poisson_memcpy_barline/poisson $N $iterations $startT $tolerance
done


exit 0
