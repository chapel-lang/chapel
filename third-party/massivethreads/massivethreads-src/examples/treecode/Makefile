CC=gcc
PROG=treecode

# add C flags and C++ flags
CC_CXX_FLAGS = -O3 -lm
CFLAGS = $(CC_CXX_FLAGS)
CXXFLAGS = $(CC_CXX_FLAGS)

SRCS = $(PROG).cc vector.cc mol.cc tree.cc opts.cc utils.cc
.PHONY: cver debug clean

all: cver

cver: treecode.c
	$(CC) -o $(PROG) $< $(CFLAGS) -L../../.libs -lmyth-compat

debug: treecode.c
	$(CC) -o $(PROG) $? -pthread -ggdb -lm

clean: 
	rm -f $(PROG) 
