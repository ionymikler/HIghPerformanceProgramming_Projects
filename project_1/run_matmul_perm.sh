#!/bin/sh

CC=${1-"gcc"}

PERMS="mnk nmk mkn nkm kmn knm"
M="50 100 500 1000 2000"

LOOPS=1000
LOGEXT=log

/bin/rm -f results.$LOGEXT
# results format
# m n k mem_usage MFlops/s

for perm in $PERMS
do
    for m in $M
    do
        echo "Running $perm with $m $((m+1)) $((m+2))"
        size="$m $((m+1)) $((m+2))"
        result=$(./drivers/matmult_c.${CC} $perm $m $((m+1)) $((m+2)))
        echo "$size $result" >> results.$LOGEXT
    done
done
# time to say 'Good bye' ;-)
#
exit 0

