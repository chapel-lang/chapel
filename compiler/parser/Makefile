COMPILER_ROOT = ..
COMPILER_SUBDIR = parser

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

PARSER_OBJDIR = $(OBJ_SUBDIR)
include Makefile.share

TARGETS = $(PARSER_OBJS)

OTHER_DEPEND_RULES = \
	@$(MAKEDEPEND) $(COMP_CXXFLAGS) lexyacc.cpp | sed s/:\ lexyacc.cpp/\ =/ | sed s/lexyacc\\\.o/LEXYACC_INCLUDES/ >> $(SVN_DEPENDS).tmp

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules

ifdef CHPL_DEVELOPER
CLEAN_TARGS += \
	*.tab.c \
	*.yy.c \
	*.tab.h \
	*.output \

endif

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
	$(OBJ_SUBDIR)/modulefinder.o \
	$(OBJ_SUBDIR)/modulefinder.tab.o \
	$(OBJ_SUBDIR)/modulefinder.lex.yy.o \
	$(OBJ_SUBDIR)/processTokens.moduleFinder.o \
o	$(OBJ_SUBDIR)/yy.o

modulefinder: modulefinder.tab.h $(MODULEFINDER_OBJS)
	g++ -o $@ $(MODULEFINDER_OBJS) $(LIBS)


FORCE:

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot


# no-error required here for bison 1.875 (not 1.875d)
$(OBJ_SUBDIR)/chapel.tab.o: chapel.tab.c $(OBJ_SUBDIR_MADE) $(LEXYACC_INCLUDES)
	$(CXX) -c $(COMP_CFLAGS) $(COMP_CFLAGS_NONCHPL) -o $@ $<

$(OBJ_SUBDIR)/modulefinder.tab.o: modulefinder.tab.c $(OBJ_SUBDIR_MADE) $(LEXYACC_INCLUDES)
	$(CXX) -c $(COMP_CFLAGS) -o $@ $<

# no-error required here for flex 2.5.33
$(OBJ_SUBDIR)/lex.yy.o: lex.yy.c $(OBJ_SUBDIR_MADE) chapel.tab.h $(LEXYACC_INCLUDES)
	$(CXX) -c $(COMP_CFLAGS) $(COMP_CFLAGS_NONCHPL) -o $@ $<

$(OBJ_SUBDIR)/modulefinder.lex.yy.o: modulefinder.lex.yy.c $(OBJ_SUBDIR_MADE) modulefinder.tab.h
	$(CXX) -c -DMODULE_FINDER $(COMP_CFLAGS) -o $@ $<

$(OBJ_SUBDIR)/processTokens.moduleFinder.o: $(OBJ_SUBDIR)/processTokens.o
	$(CXX) -c -DMODULE_FINDER $(COMP_CFLAGS) -o $@ processTokens.cpp

$(OBJ_SUBDIR)/yy.o: yy.cpp $(OBJ_SUBDIR_MADE)
	$(CXX) -c $(COMP_CFLAGS) -o $@ $<
