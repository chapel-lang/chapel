mtbb_cc    := $(wildcard *_mtbb.cc)
mtbb_dr_cc := $(wildcard *_mtbb_dr.cc)
omp_cc     := $(wildcard *_omp.cc)
omp_dr_cc  := $(wildcard *_omp_dr.cc)
cilkplus_raw_c := $(wildcard *_cilkplus_raw.c)
cilkplus_raw_dr_c := $(wildcard *_cilkplus_raw_dr.c)
cilkplus_c := $(wildcard *_cilkplus.c)
cilkplus_dr_c := $(wildcard *_cilkplus_dr.c)
cilk_cilk := $(wildcard *_cilk.cilk)
cilk_dr_cilk := $(wildcard *_cilk_dr.cilk)

mtbb_exe    := $(patsubst %.cc,%,$(mtbb_cc))
mtbb_dr_exe := $(patsubst %.cc,%,$(mtbb_dr_cc))
omp_exe     := $(patsubst %.cc,%,$(omp_cc))
omp_dr_exe  := $(patsubst %.cc,%,$(omp_dr_cc))
cilkplus_raw_exe     := $(patsubst %.c,%,$(cilkplus_raw_c))
cilkplus_raw_dr_exe  := $(patsubst %.c,%,$(cilkplus_raw_dr_c))
cilkplus_exe     := $(patsubst %.c,%,$(cilkplus_c))
cilkplus_dr_exe  := $(patsubst %.c,%,$(cilkplus_dr_c))
cilk_exe     := $(patsubst %.cilk,%,$(cilk_cilk))
cilk_dr_exe  := $(patsubst %.cilk,%,$(cilk_dr_cilk))

prefix:=$(HOME)/parallel2/sys/inst/g

all : $(mtbb_exe) $(mtbb_dr_exe) $(omp_exe) $(omp_dr_exe) $(cilkplus_raw_exe) $(cilkplus_raw_dr_exe) $(cilkplus_exe) $(cilkplus_dr_exe) $(cilk_exe) $(cilk_dr_exe)

#CC:=$(prefix)/gcc/bin/gcc
#CILKC:=$(prefix)/cilk/bin/cilkc
CC:=gcc
CXX:=g++
CILKC:=cilkc
CFLAGS := -I$(prefix)/include -O0 -g
CXXFLAGS := -std=c++0x -I$(prefix)/include -O0 -g
LDFLAGS := -L$(prefix)/lib -Wl,-R$(prefix)/lib 

$(mtbb_exe) : % : %.cc
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $^ -lmyth-native

$(mtbb_dr_exe) : % : %.cc
#	$(CXX) -o $@ $(CXXFLAGS) -DTO_SERIAL -DDAG_RECORDER=2 $(LDFLAGS) $^ -ldr -lpthread
	$(CXX) -o $@ $(CXXFLAGS) -DDAG_RECORDER=2 $(LDFLAGS) $^ -lmyth-native -ldr -lpthread

$(omp_exe) : % : %.cc
	$(CXX) -o $@ -fopenmp $(CXXFLAGS) $(LDFLAGS) $^

$(omp_dr_exe) : % : %.cc
	$(CXX) -o $@ -fopenmp $(CXXFLAGS) -DDAG_RECORDER=2 $(LDFLAGS) $^ -ldr

$(cilkplus_raw_exe) : % : %.c
	$(CC) -o $@ -fcilkplus $(CFLAGS) $(LDFLAGS) $^ -lcilkrts

$(cilkplus_raw_dr_exe) : % : %.c
	$(CC) -o $@ -fcilkplus $(CFLAGS) -DDAG_RECORDER=2 $(LDFLAGS) $^ -ldr -lcilkrts

$(cilkplus_exe) : % : %.c
	$(CC) -o $@ -fcilkplus $(CFLAGS) $(LDFLAGS) $^

$(cilkplus_dr_exe) : % : %.c
	$(CC) -o $@ -fcilkplus $(CFLAGS) -DDAG_RECORDER=2 $(LDFLAGS) $^ -ldr

$(cilk_exe) : % : %.cilk
	$(CILKC) -o $@ $(CFLAGS) $(LDFLAGS) $^

$(cilk_dr_exe) : % : %.cilk
	$(CILKC) -o $@ $(CFLAGS) -DDAG_RECORDER=2 $(LDFLAGS) $^ -ldr

