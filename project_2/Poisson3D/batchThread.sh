#!/bin/sh 
### General options 
### -- specify queue -- 
#BSUB -q hpcintro

### -- set the job Name --
#BSUB -J batchjob

### -- ask for number of cores (default: 1) -- 
#BSUB -n 24

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

threds="1 2 4 8"
# N=$(seq 10 5 100)
iterations=1000
tolerance=1e-5
startT=0
output_type=0

Ns="10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95 100"
# tolerance=0.00001

rm -f "results_thread_j_p.txt"

for N in $Ns; do
    for thread in $threds; do
        OMP_NUM_THREADS=$thread ./jacobian_paral_complex $N $iterations $tolerance $startT $thread $output_type
        # omp_set_num_threads=$thred ./jacobian_paral_simple $N $iterations $tolerance $startT $output_type

        # omp_set_num_threads=$thred ./gauss_seidel_paral_complex $N $iterations $tolerance $startT $output_type
        # omp_set_num_threads=$thred ./gauss_seidel_paral_simple $N $iterations $tolerance $startT $output_type
    done
done



exit 0
