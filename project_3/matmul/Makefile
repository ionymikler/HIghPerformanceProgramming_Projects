TARGET= libmatmult.so
OBJS = matmult_omp_lib.o

CC	= nvc
CXX	= nvc++

OPT	= -g -fast -Msafeptr -Minfo -mp=gpu -gpu=pinned -gpu=lineinfo -gpu=cc90 -cuda -mp=noautopar -cudalib=cublas
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

DEF_TYPE=mkn_omp
DEF_M=100
DEF_N=100
DEF_K=100
DEF_OMP_NUM_THREADS=4
DEF_MFLOPS_MAX_IT=1
run:
	OMP_NUM_THREADS=$(DEF_OMP_NUM_THREADS) MFLOPS_MAX_IT=$(DEF_MFLOPS_MAX_IT) ./matmult_c.nvc++ $(DEF_TYPE) $(DEF_M) $(DEF_N) $(DEF_K)
