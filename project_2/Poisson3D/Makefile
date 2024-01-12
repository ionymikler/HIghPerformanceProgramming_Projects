# Makefile
DEFAULT_TARGET = poisson_gs
TARGET_J  = poisson_j		# Jacobi
TARGET_GS = poisson_gs		# Gauss-Seidel

SOURCES	= main.c print.c alloc3d.c cube_utils.c solver.c
OBJECTS	= print.o alloc3d.o cube_utils.o
MAIN_J	= main_j.o
MAIN_GS = main_gs.o
OBJS_J	= $(MAIN_J) jacobi.o solver_j.o
OBJS_GS	= $(MAIN_GS) gauss_seidel.o solver_gs.o

# options and settings for the GCC compilers
CC	= gcc
DEFS	= 
OPT	= -g
OPT += -O3 -fopenmp -fopt-info -ffast-math -funroll-loops
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

solver_j.o:
	$(CC) -o $@ -D_JACOBI $(CFLAGS) -c solver.c

solver_gs.o:
	$(CC) -o $@ -D_GAUSS_SEIDEL $(CFLAGS) -c solver.c

clean:
	@/bin/rm -f core *.o *~

realclean: clean
	@/bin/rm -f $(TARGET_J)  $(TARGET_GS)

clean_r:
	@/bin/echo "Cleaning results directory"
	@/bin/rm -f results/*  *.vtk *.dat

# Default run values
N = 300
ITER_MAX = 1000
TOL = 1e-5
START_T = 0.0
THREAD_NUM=8
VERBOSE = 0 # 0 = no, 1 = yes
OUTPUT_TYPE=0 # 0:=None 3:='.dat' 4:='.vtk

run_j:
	OMP_NUM_THREADS=$(THREAD_NUM) ./$(TARGET_J) $(N) $(ITER_MAX) $(TOL) $(START_T) $(THREAD_NUM) $(VERBOSE) $(OUTPUT_TYPE)

run_gs:
	OMP_NUM_THREADS=$(THREAD_NUM) ./$(TARGET_GS) $(N) $(ITER_MAX) $(TOL) $(START_T) $(THREAD_NUM) $(VERBOSE) $(OUTPUT_TYPE)

run:
	OMP_NUM_THREADS=$(THREAD_NUM) ./$(DEFAULT_TARGET) $(N) $(ITER_MAX) $(TOL) $(START_T) $(THREAD_NUM) $(VERBOSE) $(OUTPUT_TYPE)

submit:
	@/bin/rm -f *.out
	bsub < testing/job_script.sh

# DO NOT DELETE

main_j.o: main.c print.h jacobi.h cube_utils.h solver.h
main_gs.o: main.c print.h gauss_seidel.h cube_utils.h solver.h
print.o: print.h