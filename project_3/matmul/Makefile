TARGET= libmatmult.so
OBJS = matmult_omp_lib.o

CC	= nvc
CXX	= nvc++

OPT	= -g -fast -Msafeptr -Minfo=all -mp=gpu -gpu=pinned -gpu=lineinfo -gpu=cc90 -cuda -mp=noautopar -cudalib=cublas
OPT += -fopenmp
PIC   = -fpic -shared
ISA	=
PARA	=
INC   =
LIBS	=

CXXFLAGS= $(OPT) $(PIC) $(INC) $(ISA) $(PARA) $(XOPT)

all: $(TARGET)

$(TARGET): $(OBJS) 
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	@/bin/rm -f $(TARGET) $(OBJS)

TYPE=asy_offload
DEF_N=500
DEF_M=$(DEF_N)
DEF_K=$(DEF_M)
DEF_OMP_NUM_THREADS=1
DEF_MFLOPS_MAX_IT=10
N=$(DEF_N)

run:
	export OMP_NUM_THREADS=$(DEF_OMP_NUM_THREADS)
	@echo "running '$(TYPE)' '$(N)'"
	MFLOPS_MAX_IT=$(DEF_MFLOPS_MAX_IT) ./matmult_c.nvc++ $(TYPE) $(N) ${N} $(N)

run_rect:
	export OMP_NUM_THREADS=$(DEF_OMP_NUM_THREADS)
	@echo "running '$(TYPE)' '$(N)' '$$((N+10))' '$$((N+20))'"
	MFLOPS_MAX_IT=$(DEF_MFLOPS_MAX_IT) ./matmult_c.nvc++ $(TYPE) 500 520 540

many:
	@for n in 500 1000 1500 2000; do \
		echo "running '$(TYPE)' '$$n' '$$((n))' '$$((n))'"; \
		MFLOPS_MAX_IT=$(DEF_MFLOPS_MAX_IT) ./matmult_c.nvc++ $(TYPE) $$n $$((n)) $$((n)); \
		echo "running '$(TYPE)' '$$n' '$$((n+10))' '$$((n+20))'"; \
		MFLOPS_MAX_IT=$(DEF_MFLOPS_MAX_IT) ./matmult_c.nvc++ $(TYPE) $$n $$((n+20)) $$((n+40)); \
	done

submit:
	bsub < mm_batch_gpu.sub
