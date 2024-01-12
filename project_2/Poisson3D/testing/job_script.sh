#!/bin/sh 
### General options 
### -- specify queue -- 
#BSUB -q hpc

### -- set the job Name --
#BSUB -J poisson3d

### -- ask for number of cores (default: 1) -- 
#BSUB -n 1

### -- specify that the cores must be on the same host -- 
#BSUB -R "span[hosts=1] affinity[socket(1)]"

### -- specify memory need per core/slot -- 
#BSUB -R "rusage[mem=2048]"

### -- set walltime limit: hh:mm -- 
#BSUB -W 90

### -- set the email address -- 
#BSUB -u s222962@dtu.dk

### -- send notification at start -- 
#BSUB -B

### -- send notification at completion -- 
#BSUB -N

### -- Specify the output and error file. %J is the job-id -- 
## -- -o and -e mean append, -oo and -eo mean overwrite -- 
#BSUB -o results/Output_%J.out
#BSUB -e results/Output_%J.err

# Fixed values
ITER_MAX=5000
TOL=1e-5
INIT_TEMP=10
THREAD_NUM=1
VERBOSE=0 # 0 = no, 1 = yes
OUTPUT_TYPE=0 # 0:=None 3:='.dat' 4:='.vtk

# Parameters
Ns="100" # Start small, check times
# THREAD_NUMS="1 2 4"
THREAD_NUMS="1 2 4"
# EXEs="poisson_j poisson_gs"
EXEs="poisson_gs"

TIME=$(date +"%H:%M:%S")
RESULTS_FILE="results/results_${TIME}.txt"

# environment info
echo "Current working directory: $(pwd)"

# check executables exist
for EXE in $EXEs; do
    if [ ! -f $EXE ]; then
        echo "Executable $EXE not found"
        exit 1
    fi
done

# run the executables
# clean results file
echo "" > $RESULTS_FILE
for EXE in $EXEs; do
    echo ""
    echo "--- Iterating with $EXE ---"
    for tn in $THREAD_NUMS; do
        echo "  Setting thread_num=$tn"
        for N in $Ns; do
            if [ -n "$N" ]; then
                echo "    Running $EXE with N=$N, thread_num=$THREAD_NUM"
                ./$EXE $N $ITER_MAX $TOL $INIT_TEMP $THREAD_NUM $VERBOSE $OUTPUT_TYPE >> $RESULTS_FILE
            fi
        done
    done
done

echo "Done"