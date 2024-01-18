#!/bin/sh 
### General options 
### -- specify queue -- 
#BSUB -q hpc

### -- set the job Name --
#BSUB -J system_info

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

# System info
SYSTEM_INFO_FILE=results/system_info.txt
# clear file
echo "" > $SYSTEM_INFO_FILE
echo "System info:" >> $SYSTEM_INFO_FILE
echo "--- lscpu ---" >> $SYSTEM_INFO_FILE
lscpu >> $SYSTEM_INFO_FILE
echo "---" >> $SYSTEM_INFO_FILE
echo "--- nvidia-smi ---" >> $SYSTEM_INFO_FILE
nvidia-smi >> $SYSTEM_INFO_FILE
echo "---" >> $SYSTEM_INFO_FILE
echo "--- nvaccelinfo ---" >> $SYSTEM_INFO_FILE
nvaccelinfo >> $SYSTEM_INFO_FILE