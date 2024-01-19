#!/bin/sh 
### General options 
### -- specify queue -- 
#BSUB -q hpcintrogpu

### -- set the job Name --
#BSUB -J batchjob

### -- ask for number of cores (default: 1) -- 
#BSUB -n 32

#BSUB -gpu "num=2:mode=exclusive_process"
#BSUB -R "span[hosts=1]"

### -- specify memory need per core/slot -- 
#BSUB -R "rusage[mem=2048]"

### -- set walltime limit: hh:mm -- 
#BSUB -W 40

### -- send notification at start -- 
#BSUB -B

### -- send notification at completion -- 
#BSUB -N

### -- Specify the output and error file. %J is the job-id -- 
### -- -o and -e mean append, -oo and -eo mean overwrite -- 
##BSUB -o Output_%J.out
##BSUB -e Output_%J.err 

module load /appl9/nvhpc/2023_2311/modulefiles/nvhpc-nompi/23.11


Ns="30 40 50 60 70 80 90 100 110 120 130 140 150 160 170 180 190 200"
iterations=1000
startT=0
tolerance=0.1
threds=8

# === Warm up device
warmUpDevice
for N in $Ns
do
    ./Poisson_map/poisson $N $iterations $startT
done

rm -f "results_mem_2D.txt"
rm -f "results_base.txt"
rm -f "results_base_ES.txt"
rm -f "results.txt"
rm -f "results_mem.txt"
rm -f "results_ES.txt"

# === Compare data transfering strategies
export CUDA_VISIBLE_DEVICES=0,1

for N in $Ns
do
    OMP_NUM_THREADS=$threds ./Poisson_baseline/poisson_j $N $iterations $tolerance $startT $threds 0 0 # cpu parralize
    ./Poisson_map/poisson $N $iterations $startT
    ./Poisson_mem/poisson $N $iterations $tolerance $startT $threds 0 0
    ./Poisson_mem_2D/poisson $N $iterations $tolerance $startT $threds 0 0
done


# === compare gpu and cpu parallization with stopping criteria

# for N in $Ns
# do
#     OMP_NUM_THREADS=$threds ./Poisson_baseline_ES/poisson_j $N $iterations $tolerance $startT $threds 0 0
#     ./Poisson_map_ES/poisson $N $iterations $startT $tolerance
# done

# === compare 1 gpu with 2 gpus



# scp -r s183778@transfer.gbar.dtu.dk:/zhome/ad/0/137980/hpc_course_projects/project_3/poisson/Poisson_mem C:/Users/tore_/Downloads/Poisson_mem
# scp -r C:/Users/tore_/Downloads/Poisson_mem s183778@transfer.gbar.dtu.dk:/zhome/ad/0/137980/hpc_course_projects/project_3/poisson/Poisson_mem; \


exit 0
