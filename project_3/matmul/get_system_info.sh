#!/bin/bash
# 02614 - High-Performance Computing, January 2024
# 
# batch script to run matmult on a dedicated GPU server in the hpcintrogpu
# queue
#
# Author: Bernd Dammann <bd@cc.dtu.dk>
#         Hans Henrik Brandenborg Sørensen <hhbs@dtu.dk>
#
#BSUB -J system_info
#BSUB -q hpcintrogpu
#BSUB -o results/system_info_%J.out

### -- ask for number of cores (default: 1) -- 
# QUESTION: cores to the maximum? relation to teams per core?
#BSUB -n 32

#BSUB -R "rusage[mem=2048]"
#BSUB -W 15
#BSUB -R "span[hosts=1]"
#BSUB -gpu "num=1:mode=exclusive_process"

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