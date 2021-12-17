CHPL_MAKE_HOME = /home/arezaii/git/chapel

CHPL_MAKE_RUNTIME_LIB = /home/arezaii/git/chapel/lib

CHPL_MAKE_RUNTIME_INCL = /home/arezaii/git/chapel/runtime/include

CHPL_MAKE_THIRD_PARTY = /home/arezaii/git/chapel/third-party

TMPDIRNAME = ccode

export CHPL_MAKE_CHPLENV_CACHE := CHPL_MAKE_ATOMICS=cstdlib|CHPL_MAKE_AUX_FILESYS=none|CHPL_MAKE_COMM=none|CHPL_MAKE_COMM_SUBSTRATE=none|CHPL_MAKE_COMPILER_SUBDIR=linux64/gnu/x86_64/hostmem-cstdlib/llvm-system/san-none|CHPL_MAKE_CUDA_PATH=|CHPL_MAKE_GASNET_SEGMENT=none|CHPL_MAKE_GASNET_UNIQ_CFG_PATH=linux64-x86_64-native-llvm-none/substrate-none/seg-none|CHPL_MAKE_GMP=none|CHPL_MAKE_GMP_UNIQ_CFG_PATH=linux64-x86_64-native-llvm-none|CHPL_MAKE_GPU_CODEGEN=none|CHPL_MAKE_HOME=/home/arezaii/git/chapel|CHPL_MAKE_HOST_ARCH=x86_64|CHPL_MAKE_HOST_BIN_SUBDIR=linux64-x86_64|CHPL_MAKE_HOST_CC=gcc|CHPL_MAKE_HOST_COMPILER=gnu|CHPL_MAKE_HOST_CPU=none|CHPL_MAKE_HOST_CXX=g++|CHPL_MAKE_HOST_JEMALLOC=none|CHPL_MAKE_HOST_MEM=cstdlib|CHPL_MAKE_HOST_PLATFORM=linux64|CHPL_MAKE_HWLOC=none|CHPL_MAKE_HWLOC_UNIQ_CFG_PATH=linux64-x86_64-native-llvm-none-flat|CHPL_MAKE_JEMALLOC_UNIQ_CFG_PATH=linux64-x86_64-native-llvm-none|CHPL_MAKE_LAUNCHER=none|CHPL_MAKE_LAUNCHER_SUBDIR=linux64/gnu/x86_64/loc-flat/comm-none/tasks-fifo/launch-none/tmr-generic/unwind-none/mem-cstdlib/atomics-cstdlib/lib_pic-none/san-none|CHPL_MAKE_LIBFABRIC=none|CHPL_MAKE_LIBFABRIC_UNIQ_CFG_PATH=linux64-x86_64-native-llvm-none|CHPL_MAKE_LIBUNWIND_UNIQ_CFG_PATH=linux64-x86_64-native-llvm-none|CHPL_MAKE_LIB_PIC=none|CHPL_MAKE_LLVM=system|CHPL_MAKE_LLVM_CLANG_C=/usr/lib/llvm-11/bin/clang|CHPL_MAKE_LLVM_CLANG_COMPILE_ARGS=|CHPL_MAKE_LLVM_CLANG_CXX=/usr/lib/llvm-11/bin/clang++|CHPL_MAKE_LLVM_CLANG_LINK_ARGS=|CHPL_MAKE_LLVM_CONFIG=llvm-config-11|CHPL_MAKE_LLVM_UNIQ_CFG_PATH=system|CHPL_MAKE_LOCALE_MODEL=flat|CHPL_MAKE_MAKE=make|CHPL_MAKE_MEM=cstdlib|CHPL_MAKE_NETWORK_ATOMICS=none|CHPL_MAKE_PE_CHPL_PKGCONFIG_LIBS=|CHPL_MAKE_QTHREAD_UNIQ_CFG_PATH=linux64-x86_64-native-llvm-none-flat-cstdlib-none|CHPL_MAKE_RE2=none|CHPL_MAKE_RE2_UNIQ_CFG_PATH=linux64-x86_64-native-llvm-none|CHPL_MAKE_RUNTIME_CPU=native|CHPL_MAKE_RUNTIME_INCL=/home/arezaii/git/chapel/runtime/include|CHPL_MAKE_RUNTIME_LIB=/home/arezaii/git/chapel/lib|CHPL_MAKE_RUNTIME_SUBDIR=linux64/llvm/x86_64/cpu-native/loc-flat/comm-none/tasks-fifo/tmr-generic/unwind-none/mem-cstdlib/atomics-cstdlib/hwloc-none/re2-none/fs-none/lib_pic-none/san-none|CHPL_MAKE_SANITIZE=none|CHPL_MAKE_SANITIZE_EXE=none|CHPL_MAKE_SYS_MODULES_SUBDIR=linux64-x86_64-llvm|CHPL_MAKE_TARGET_ARCH=x86_64|CHPL_MAKE_TARGET_BACKEND_CPU=native|CHPL_MAKE_TARGET_BIN_SUBDIR=linux64-x86_64-native|CHPL_MAKE_TARGET_CC=/usr/lib/llvm-11/bin/clang|CHPL_MAKE_TARGET_COMPILER=llvm|CHPL_MAKE_TARGET_COMPILER_PRGENV=none|CHPL_MAKE_TARGET_CPU=native|CHPL_MAKE_TARGET_CPU_FLAG=arch|CHPL_MAKE_TARGET_CXX=/usr/lib/llvm-11/bin/clang++|CHPL_MAKE_TARGET_JEMALLOC=none|CHPL_MAKE_TARGET_MEM=cstdlib|CHPL_MAKE_TARGET_PLATFORM=linux64|CHPL_MAKE_TASKS=fifo|CHPL_MAKE_THIRD_PARTY=/home/arezaii/git/chapel/third-party|CHPL_MAKE_THIRD_PARTY_COMPILE_ARGS=|CHPL_MAKE_THIRD_PARTY_HOST_LINK_ARGS=|CHPL_MAKE_THIRD_PARTY_LINK_ARGS=-ldl|CHPL_MAKE_TIMERS=generic|CHPL_MAKE_UNWIND=none|

SKIP_COMPILE_LINK = skip
BINNAME = lib/libinteropWithC.a

TMPBINNAME = ccode/libinteropWithC.tmp.a
COMP_GEN_WARN = 0
COMP_GEN_DEBUG = 0
COMP_GEN_OPT = 0
COMP_GEN_SPECIALIZE = 0
COMP_GEN_FLOAT_OPT = 0
COMP_GEN_USER_CFLAGS =  -I/home/arezaii/git/chapel/modules/internal 
COMP_GEN_LFLAGS = $(LIB_STATIC_FLAG)
COMP_GEN_USER_LDFLAGS = 
TAGS_COMMAND = 

CHPLSRC = \
	ccode/chpl__module.o \

CHPLUSEROBJ = \

CHPL_CL_C_SRCS = \
	cHelper.c \
	fact.c \

CHPL_CL_OBJS = \
	ccode/cHelper.c.o \
	ccode/fact.c.o \


LIBS = $(LIBMVEC)


include $(CHPL_MAKE_HOME)/runtime/etc/Makefile.static

ccode/cHelper.c.o: cHelper.c FORCE
	$(CC) -c -o $@ $(GEN_CFLAGS) $(COMP_GEN_CFLAGS) $(CHPL_RT_INC_DIR) $<

ccode/fact.c.o: fact.c FORCE
	$(CC) -c -o $@ $(GEN_CFLAGS) $(COMP_GEN_CFLAGS) $(CHPL_RT_INC_DIR) $<


