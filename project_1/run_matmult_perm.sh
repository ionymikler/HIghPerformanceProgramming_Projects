#!/bin/sh 
### General options 
### -- specify queue -- 
#BSUB -q hpc

### -- set the job Name -- 
#BSUB -J matmul_perms

### -- ask for number of cores (default: 1) -- 
#BSUB -n 1

### -- specify that the cores must be on the same host -- 
#BSUB -R "span[hosts=1] affinity[socket(1)]"

### -- specify memory need per core/slot -- 
#BSUB -R "rusage[mem=2048]"

### -- set walltime limit: hh:mm -- 
#BSUB -W 60

### -- set the email address -- 
#BSUB -u s222962@dtu.dk

### -- send notification at start -- 
#BSUB -B

### -- send notification at completion -- 
#BSUB -N

### -- Specify the output and error file. %J is the job-id -- 
### -- -o and -e mean append, -oo and -eo mean overwrite -- 
##BSUB -o Output_%J.out
##BSUB -e Output_%J.err 

CC=${1-"gcc"}

# Setup for profiling and comparison
export MFLOPS_MAX_IT=2

SO_FILE="libmatmult_base.so"
SO_FILE_WITHOUT_EXT=$(basename "$SO_FILE" .so)

RESULTS_FILE="results/${SO_FILE_WITHOUT_EXT}_results.dat"
LOG_FILE="results/${SO_FILE_WITHOUT_EXT}.log"

PERMS="mnk nmk mkn nkm kmn knm"
M="5 10 50 100 250 500 750 1000 1500 2000"

# PERMS="mnk"
# M="5 10"

# cleanup of past runs of the same config
rm -f ${RESULTS_FILE} ${LOG_FILE} "${SO_FILE_WITHOUT_EXT}.png"

# bring the correct .so file
rm -f "libmatmult.so"
if [ ! -f "./exes/${SO_FILE}" ]; then
    echo "Error: ./exes/${SO_FILE} does not exist" 2> $LOG_FILE
    exit 1
fi
cp "./exes/${SO_FILE}" "./libmatmult.so"


if [ ! -f "./libmatmult.so" ]; then
    echo "Error:  libmatmul.so does not exist" 2> $LOG_FILE
    exit 1
fi

echo "Running driver on ${SO_FILE} | $(date)
" >> $LOG_FILE
for perm in $PERMS
do
    for m in $M
    do
        echo "Running $perm with $m $((m+1)) $((m+2)) | $(date)" >> $LOG_FILE
        size="$m $((m+1)) $((m+2))"
        result=$(./drivers/matmult_c.${CC} $perm $m $((m+1)) $((m+2)))
        echo "$size $result" >> "${RESULTS_FILE}" # results format: m n k mem_usage MFlops/s
    done
done

rm -f "libmatmult.so"
echo "
Done. removing libmatmult.so file" >> $LOG_FILE

exit 0

