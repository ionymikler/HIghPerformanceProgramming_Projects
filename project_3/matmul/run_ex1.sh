#!/bin/sh

### General options 
### -- specify queue -- 
#BSUB -q hpcintrogpu

### -- set the job Name --
#BSUB -J matmul_mkn_omp

### -- ask for number of cores (default: 1) -- 
#BSUB -n 1

### -- specify that the cores must be on the same host -- 
#BSUB -R "span[hosts=1] affinity[socket(1)]"

### -- specify memory need per core/slot -- 
#BSUB -R "rusage[mem=2048]"

### -- set walltime limit: hh:mm -- 
#BSUB -W 90

### -- set the email address -- 
#BSUB -u s222695@dtu.dk

### -- send notification at start -- 
#BSUB -B

### -- send notification at completion -- 
#BSUB -N

### -- Specify the output and error file. %J is the job-id -- 
### -- -o and -e mean append, -oo and -eo mean overwrite -- 
##BSUB -o Output_%J.out
##BSUB -e Output_%J.err 

M="5 10 50 100 250 500 750 1000 1500 2000 2500 3000 5000"
LOGEXT=log
DEF_OMP_NUM_THREADS=4
DEF_MFLOPS_MAX_IT=1

/bin/rm -f results_ex1_mkn_omp.$LOGEXT
/bin/rm -f results_ex1_lib.$LOGEXT
/bin/rm -f results_infra.$LOGEXT
# results format
# m n k mem_usage MFlops/s


echo $(lscpu) >> results_infra.$LOGEXT

for m in $M
do
        echo "Running mkn_omp with $m $((m+1)) $((m+2))"
        size="$m $((m+1)) $((m+2))"
        result=$(OMP_NUM_THREADS=${DEF_OMP_NUM_THREADS} MFLOPS_MAX_IT=${DEF_MFLOPS_MAX_IT} ./matmult_c.nvc++ mkn_omp $m $((m+1)) $((m+2))) 
        echo "$size $result" >> results_ex1_mkn_omp.$LOGEXT
done

for m in $M
do
        echo "Running lib with $m $((m+1)) $((m+2))"
        size="$m $((m+1)) $((m+2))"
        result=$(OMP_NUM_THREADS=${DEF_OMP_NUM_THREADS} MFLOPS_MAX_IT=${DEF_MFLOPS_MAX_IT} ./matmult_c.nvc++ lib $m $((m+1)) $((m+2))) 
        echo "$size $result" >> results_ex1_lib.$LOGEXT
done

# time to say 'Good bye' ;-)
#
exit 0

