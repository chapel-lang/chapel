COMPILER_ROOT = ..
COMPILER_SUBDIR = parser

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

PARSER_OBJDIR = $(PLATFORM)
include Makefile.share

TARGETS = $(PARSER_OBJS)

OTHER_DEPEND_RULES = \
	@$(MAKEDEPEND) -MT LEXYACC_INCLUDES $(COMP_CXXFLAGS) lexyacc.cpp | sed s/:\ lexyacc.cpp/\ =/ | grep -v \^\# >> $(CVS_DEPENDS).tmp

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules

CLEAN_TARGS += \
	*.tab.c \
	*.yy.c \
	*.tab.h \
	*.output \

lex.yy.c: chapel.lex chapel.tab.h
	flex chapel.lex

modulefinder.lex.yy.c: modulefinder.lex modulefinder.tab.h
	flex -o$@ modulefinder.lex

#BLC: This rule uses two hacks: 
#     - one to avoid using return rather than $$ which has cost me hours 
#       of frustration;  
#     - the second to get rid of some problematic generated code with a
#       particular version of bison (will hopefully go away)
chapel.tab.h chapel.tab.c: chapel.y
	bison -d -v -t chapel.y
	cat chapel.tab.c | sed s/__attribute__/\\/\\// > chapel.tab.c.tmp
	mv chapel.tab.c.tmp chapel.tab.c
	@if [ `grep "conflicts:" chapel.output | wc -l` -ne 0 ]; then echo "PROBLEM: chapel.y contains conflicts"; exit 1; fi;
#	@if [`grep return chapel.y | wc -l` -ne 0 ]; then echo "PROBLEM: chapel.y contains return"; grep zyxv chapel.y; fi;

modulefinder.tab.h modulefinder.tab.c: modulefinder.y
	bison -d -v -t modulefinder.y
	cat modulefinder.tab.c | sed s/__attribute__/\\/\\// > modulefinder.tab.c.tmp
	mv modulefinder.tab.c.tmp modulefinder.tab.c
	@if [ `grep "conflicts:" modulefinder.output | wc -l` -ne 0 ]; then echo "PROBLEM: modulefinder.y contains conflicts"; exit 1; fi;

MODULEFINDER_OBJS = \
	$(PLATFORM)/modulefinder.o \
	$(PLATFORM)/modulefinder.tab.o \
	$(PLATFORM)/modulefinder.lex.yy.o \
	$(PLATFORM)/processTokens.moduleFinder.o \
	$(PLATFORM)/yy.o

modulefinder: modulefinder.tab.h $(MODULEFINDER_OBJS)
	g++ -o $@ $(MODULEFINDER_OBJS) $(LIBS)


FORCE:

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot


$(PLATFORM)/chapel.tab.o: chapel.tab.c $(PLATFORM_DIR) $(LEXYACC_INCLUDES)
	$(CXX) -c $(COMP_CFLAGS) -Wno-error -o $@ $<

$(PLATFORM)/modulefinder.tab.o: modulefinder.tab.c $(PLATFORM_DIR) $(LEXYACC_INCLUDES)
	$(CXX) -c $(COMP_CFLAGS) -Wno-error -o $@ $<

$(PLATFORM)/chapel.tab.nogc.o: chapel.tab.c $(PLATFORM_DIR) $(LEXYACC_INCLUDES)
	$(CXX) -c $(COMP_CFLAGS) -Wno-error -o $@ $<

$(PLATFORM)/lex.yy.o: lex.yy.c $(PLATFORM_DIR) chapel.tab.h $(LEXYACC_INCLUDES)
	$(CXX) -c $(COMP_CFLAGS) -o $@ $<

$(PLATFORM)/modulefinder.lex.yy.o: modulefinder.lex.yy.c $(PLATFORM_DIR) modulefinder.tab.h
	$(CXX) -c -DMODULE_FINDER $(COMP_CFLAGS) -o $@ $<

$(PLATFORM)/processTokens.moduleFinder.o: $(PLATFORM)/processTokens.o
	$(CXX) -c -DMODULE_FINDER $(COMP_CFLAGS) -o $@ processTokens.cpp

$(PLATFORM)/lex.yy.nogc.o: lex.yy.c $(PLATFORM_DIR) chapel.tab.h $(LEXYACC_INCLUDES)
	$(CXX) -c $(COMP_CFLAGS) -o $@ $<

$(PLATFORM)/yy.o: yy.c $(PLATFORM_DIR)
	$(CC) -c $(COMP_CFLAGS) -o $@ $<

$(PLATFORM)/yy.nogc.o: yy.c $(PLATFORM_DIR)
	$(CC) -c $(COMP_CFLAGS) -o $@ $<
