COMPILER_ROOT = ..
COMPILER_SUBDIR = AST

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

CFLAGS = -g -Wall -Werror -I$(GC_ROOT)/include/gc
INCL = -I$(GC_ROOT)/include/gc

CXX_SRCS = \
	expr.cpp \
	link.cpp \
	loc.cpp \
	main.cpp \
	stmt.cpp \
	stringutil.cpp \
	symbol.cpp \
        symtab.cpp \
	type.cpp \

C_SRCS = \
	yy.c \

CXX_C_SRCS = \
	chapel.tab.c \
	lex.yy.c \

CVS_SRCS = $(CXX_SRCS) $(C_SRCS)
SRCS = $(CVS_SRCS) $(CXX_C_SRCS)

OBJS=$(C_SRCS:%.c=$(PLATFORM)/%.o) $(CXX_C_SRCS:%.c=$(PLATFORM)/%.o) $(CXX_SRCS:%.cpp=$(PLATFORM)/%.o)

all: bradast

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules

bradast: $(OBJS)
	$(CXX) -o bradast $(OBJS) $(GC_ROOT)/lib/libgc.a
	@etags $(SRCS) *.h

CLEAN_TARGS += \
	chapel.tab.c \
	lex.yy.c \
	chapel.tab.h \
	chapel.output \

lex.yy.c: chapel.lex chapel.tab.h
	flex chapel.lex

chapel.tab.h chapel.tab.c: chapel.y
	bison -d -v -t chapel.y
	cat chapel.tab.c | sed s/__attribute__/\\/\\// > chapel.tab.c.tmp
	mv chapel.tab.c.tmp chapel.tab.c


$(PLATFORM)/chapel.tab.o: chapel.tab.c $(PLATFORM_DIR) *.h
	$(CC) -c -g $(INCL) -o $@ $<

$(PLATFORM)/lex.yy.o: lex.yy.c $(PLATFORM_DIR) *.h
	$(CC) -c -g $(INCL) -o $@ $<

$(PLATFORM)/yy.o: yy.c $(PLATFORM_DIR) yy.h *.h
	gcc -c -g $(INCL) -o $@ $<

FORCE:

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
