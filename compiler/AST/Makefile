COMPILER_ROOT = ..
COMPILER_SUBDIR = AST

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

INCL = -I. -I.. -I$(GC_ROOT)/include/gc -I$(D_PARSER_INC_DIR)
CFLAGS = -g -Wall -Werror $(INCL) -DUSE_GC

AST_OBJDIR = $(PLATFORM)
include Makefile.share

all: $(AST_OBJS)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules

bradast: $(AST_OBJS) $(PLATFORM)/main.o $(PLATFORM)/stringutil.o
	$(CXX) -o bradast $(AST_OBJS) $(PLATFORM)/main.o $(PLATFORM)/stringutil.o $(GC_ROOT)/lib/libgc.a
	@etags $(SRCS) *.h

CLEAN_TARGS += \
	chapel.tab.c \
	lex.yy.c \
	chapel.tab.h \
	chapel.output \

lex.yy.c: chapel.lex chapel.tab.h
	flex chapel.lex

#BLC: This rule uses two hacks: 
#     - one to avoid using return rather than $$ which has cost me hours 
#       of frustration;  
#     - the second to get rid of some problematic generated code with a
#       particular version of bison (will hopefully go away)
chapel.tab.h chapel.tab.c: chapel.y
	@if [`grep return chapel.y | wc -l` -ne 0 ]; then echo "PROBLEM: chapel.y contains return"; grep zyxv chapel.y; fi;
	bison -d -v -t chapel.y
	cat chapel.tab.c | sed s/__attribute__/\\/\\// > chapel.tab.c.tmp
	mv chapel.tab.c.tmp chapel.tab.c


$(PLATFORM)/chapel.tab.o: chapel.tab.c $(PLATFORM_DIR) *.h
	$(CXX) -c -g $(INCL) -o $@ $<

$(PLATFORM)/lex.yy.o: lex.yy.c $(PLATFORM_DIR) *.h
	$(CXX) -c -g $(INCL) -o $@ $<

$(PLATFORM)/yy.o: yy.c $(PLATFORM_DIR) yy.h *.h
	$(CC) -c -g $(INCL) -o $@ $<

FORCE:

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
