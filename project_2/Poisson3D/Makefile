# Makefile
#
# .PHONY: all clean realclean get_gs_seq get_gs_psimple
N = 20
TARGET_J  = poisson_j		# Jacobi
TARGET_GS = poisson_gs		# Gauss-Seidel

SOURCES	= main.c print.c alloc3d.c cube_utils.c
OBJECTS	= print.o alloc3d.o cube_utils.o
MAIN_J	= main_j.o
MAIN_GS = main_gs.o
OBJS_J	= $(MAIN_J) jacobi.o
OBJS_GS	= $(MAIN_GS) gauss_seidel.o

# options and settings for the GCC compilers
#
CC	= gcc
DEFS	= 
OPT	= -g
OPT += -O3 -fopenmp -fopt-info
IPO	= 
ISA	= 
CHIP	= 
ARCH	= 
PARA	= 
CFLAGS	= $(DEFS) $(ARCH) $(OPT) $(ISA) $(CHIP) $(IPO) $(PARA) $(XOPTS)
LDFLAGS = -lm 

all: $(TARGET_J) $(TARGET_GS) 

$(TARGET_J): $(OBJECTS) $(OBJS_J)
	$(CC) -o $@ $(CFLAGS) $(OBJS_J) $(OBJECTS) $(LDFLAGS)

$(TARGET_GS): $(OBJECTS) $(OBJS_GS)
	$(CC) -o $@ $(CFLAGS) $(OBJS_GS) $(OBJECTS) $(LDFLAGS)

$(MAIN_J):
	$(CC) -o $@ -D_JACOBI $(CFLAGS) -c main.c 

$(MAIN_GS):
	$(CC) -o $@ -D_GAUSS_SEIDEL $(CFLAGS) -c main.c 

clean:
	@/bin/rm -f core *.o *~ *.vtk

realclean: clean
	@/bin/rm -f $(TARGET_J)  $(TARGET_GS)

# custom target
get_gs_seq:
	@/bin/echo "replacing gauss_seidel.c with gauss_seidel_seq.c"
	@/bin/cp -f gauss_seidel_seq.c gauss_seidel.c

get_gs_psimple:
	@/bin/echo "replacing gauss_seidel.c with gauss_seidel_psimple.c"
	@/bin/cp -f gauss_seidel_psimple.c gauss_seidel.c



# DO NOT DELETE

main_j.o: main.c print.h jacobi.h cube_utils.h
main_gs.o: main.c print.h gauss_seidel.h cube_utils.h
print.o: print.h