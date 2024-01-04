TARGET	= exes/libmatmult_O3_fast-math_unroll-loops.so
LIBSRCS	= matrix_lib/matmul_permutations.c
LIBOBJS	= $(LIBSRCS:.c=.o)

OPT	= -g
OPT += -v
OPT += -fopt-info -O3 -ffast-math -funroll-loops

PIC	= -fPIC

CC	= gcc
CFLAGS= $(OPT) $(PIC) $(XOPTS)

SOFLAGS = -shared 
XLIBS	= 

$(TARGET): $(LIBOBJS)
	$(CC) -o $@ $(SOFLAGS) $(LIBOBJS) $(XLIBS)

clean:
	@/bin/rm -f core core.* $(LIBOBJS) *.out

submit:
	@/bin/rm -f *.out
	bsub < run_matmult_perm.sh
