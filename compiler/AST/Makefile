CC = g++

CXXFLAGS = -g -Wall -Werror

CXX_SRCS = \
	expr.cpp \
	link.cpp \
	main.cpp \
	stmt.cpp \
	stringutil.cpp \
	symbol.cpp \
	type.cpp \

CXX_C_SRCS = \
	chapel.tab.c \
	lex.yy.c \

C_SRCS = \
	yy.c \

OBJS=$(C_SRCS:.c=.o) $(CXX_C_SRCS:.c=.o) $(CXX_SRCS:.cpp=.o)

all: bradast

bradast: $(OBJS)
	$(CXX) -o bradast $(OBJS)

clean: FORCE
	rm -f *.o chapel.tab.c chapel.tab.cpp lex.yy.c chapel.tab.h chapel.output *.cppout.c chapel.tab.hpp

clobber: clean
	rm *~

lex.yy.c: chapel.lex chapel.tab.h
	flex chapel.lex

chapel.tab.cpp: chapel.ypp
	bison -d -v -t chapel.ypp

chapel.tab.h chapel.tab.c: chapel.y
	bison -d -v -t chapel.y
	cat chapel.tab.c | sed s/__attribute__/\\/\\// > chapel.tab.c.tmp
	mv chapel.tab.c.tmp chapel.tab.c


chapel.tab.o: chapel.tab.c
	$(CC) -c -g chapel.tab.c

yy.o: yy.c yy.h
	gcc -c yy.c

FORCE:
