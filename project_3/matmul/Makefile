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

TYPE=mkn_offload
DEF_N=500
DEF_M=$(DEF_N)
DEF_K=$(DEF_M)
DEF_OMP_NUM_THREADS=8
DEF_MFLOPS_MAX_IT=10
N=$(DEF_N)

run:
	@echo "running '$(TYPE)' '$(N)'"
	MFLOPS_MAX_IT=$(DEF_MFLOPS_MAX_IT) ./matmult_c.nvc++ $(TYPE) $(N) ${N} $(N)

submit:
	bsub < mm_batch_gpu.sub
