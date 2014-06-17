CXXFLAGS:=-std=c++0x -g -pthread -ggdb -Wall
CXX:=g++
MASSIVE_INST_DIR:=../../../inst
LIBS:=-L$(MASSIVE_INST_DIR)/lib -Wl,-R$(MASSIVE_INST_DIR)/lib -lmyth -ldl

fib : fib.o
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)
