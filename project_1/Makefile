TARGET	= exes/libmatmult_base.so
TARGET2	= exes/libmatmult_O3.so
TARGET3	= exes/libmatmult_O3_fast-math_unroll-loops.so

LIBSRCS	= matrix_lib/matmul_permutations.c
#LIBOBJS	= $(LIBSRCS:.c=.o)
LIBOBJS = matrix_lib/matmul_permutations.o

# OPT	= -g
# OPT = -g -O3
OPT = -g -O3 -ffast-math -funroll-loops
# OPT += -fopt-info

PIC	= -fPIC

CC	= gcc
CFLAGS= $(OPT) $(PIC) $(XOPTS)

SOFLAGS = -shared 
XLIBS	= 

# all: $(TARGET) $(TARGET2) $(TARGET3)
all: $(TARGET3)

$(TARGET): $(LIBOBJS)
	$(CC) -o $@ $(OPT) $(SOFLAGS) $(LIBOBJS) $(XLIBS)

$(TARGET2) : $(LIBOBJS)
	$(CC) -o $@ $(OPT) $(SOFLAGS) $(LIBOBJS)

$(TARGET3) : $(LIBOBJS)
	$(CC) -o $@ $(OPT) $(SOFLAGS) $(LIBOBJS)

clean:
	@/bin/rm -f core core.* $(LIBOBJS)

clean_all:
	@/bin/rm -f core core.* $(LIBOBJS) *.out exes/*

clean_res:
	@/bin/rm -f results/*

submit:
	@/bin/rm -f *.out
	bsub < matmult_perm.sh

run:
	bash ./matmult_perm.sh
