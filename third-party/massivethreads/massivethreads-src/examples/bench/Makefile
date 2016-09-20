CC=gcc

# add C flags and C++ flags
CC_CXX_FLAGS=-O3 
CFLAGS=$(CC_CXX_FLAGS)
CXXFLAGS=$(CC_CXX_FLAGS)

.PHONY: debug clean

all: mm tree hybrid_tree tmfunc simple

mm: mm.c
	$(CC) -o $@ $< $(CC_CXX_FLAGS) -L../../.libs -lmyth-compat

tree: tree.c
	$(CC) -o $@ $< $(CC_CXX_FLAGS) -L../../.libs -lmyth-compat -lm

hybrid_tree: hybrid_tree.c
	$(CC) -o $@ $< $(CC_CXX_FLAGS) -L../../.libs -lmyth-compat -lm

tmfunc: tmfunc.c
	$(CC) -o $@ $< $(CC_CXX_FLAGS) -L../../.libs -lmyth-compat -lm

simple: simple.c
	$(CC) -o $@ $< $(CC_CXX_FLAGS) -L../../.libs -lmyth-compat

debug: mm_debug tree_debug hybrid_tree_debug simple_debug

mm_debug: mm.c
	$(CC) -o $@ $< -pthread -ggdb $(CC_CXX_FLAGS)

tree_debug: tree.c
	$(CC) -o $@ $< -pthread -ggdb -lm $(CC_CXX_FLAGS)

hybrid_tree_debug: hybrid_tree.c
	$(CC) -o $@ $< -pthread -ggdb -lm $(CC_CXX_FLAGS)

simple_debug: simple.c
	$(CC) -o $@ $< -pthread -ggdb -lm $(CC_CXX_FLAGS)

clean: 
	rm -f mm tree hybrid_tree tmfunc simple
