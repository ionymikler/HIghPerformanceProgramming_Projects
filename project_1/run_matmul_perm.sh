#!/bin/sh

CC=${1-"gcc"}

# PERMS="mnk nmk mkn nkm kmn knm"
# M="5 10 50 100 500 1000 1500 2000"
PERMS="mnk"
M="5 10"

LOGEXT=log

/bin/rm -f results_test.$LOGEXT
# results format
# m n k mem_usage MFlops/s

for perm in $PERMS
do
    for m in $M
    do
        echo "Running $perm with $m $((m+1)) $((m+2)) Current time: $(date)"
        size="$m $((m+1)) $((m+2))"
        result=$(./drivers/matmult_c.${CC} $perm $m $((m+1)) $((m+2)))
        echo "$size $result" >> results.$LOGEXT
    done
done
# time to say 'Good bye' ;-)
#
exit 0

