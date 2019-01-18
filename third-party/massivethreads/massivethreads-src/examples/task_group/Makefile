## set MASSIVE_INST_DIR so it points to where
## you installed MassiveThreads (i.e., what you gave
## to --prefix)
MASSIVE_INST_DIR:=/home/tau/parallel2/sys

CXXFLAGS:=-std=c++0x -g -pthread -ggdb -Wall -I$(MASSIVE_INST_DIR)/include
CXX:=g++
LIBS:=-L$(MASSIVE_INST_DIR)/lib -Wl,-R$(MASSIVE_INST_DIR)/lib -lmyth -ldl

fib : fib.o
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)
