#!/bin/sh

CC=${1-"gcc"}

M="5 10 50 100 500 1000 1500 2000"
BS="2 10 50 100 500 1000 1500"
LOGEXT=log

/bin/rm -f results_blk.$LOGEXT
# results format
# m n k mem_usage MFlops/s

for m in $M
do
    for bs in $BS
    do
        echo "Running blk with $m $((m+1)) $((m+2)) with bs $bs"
        size="size: $m $((m+1)) $((m+2)) bs: $bs"
        result=$(./drivers/matmult_c.${CC} blk $m $((m+1)) $((m+2)) $bs) 
        echo "$size $result" >> results_blk.$LOGEXT
    done
done

# time to say 'Good bye' ;-)
#
exit 0

