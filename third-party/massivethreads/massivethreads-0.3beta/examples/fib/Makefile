CC=gcc

# add C flags and C++ flags
CC_CXX_FLAGS = -O3 
CFLAGS = $(CC_CXX_FLAGS)
CXXFLAGS = $(CC_CXX_FLAGS)

.PHONY: debug clean

all: fib fib2

fib: fib.c
	$(CC) -o $@ $< $(CC_CXX_FLAGS) -L../../.libs -lmyth-compat

fib2: fib2.c
	$(CC) -o $@ $< $(CC_CXX_FLAGS) -L../../.libs -lmyth-compat

debug:
	$(CC) -o fib fib.c -pthread -ggdb $(CC_CXX_FLAGS)
	$(CC) -o fib2 fib2.c -pthread -ggdb $(CC_CXX_FLAGS)

clean: 
	rm -f fib fib2
