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
	@$(MAKEDEPEND) -MT LEXYACC_INCLUDES $(CXXFLAGS) lexyacc.cpp | sed s/:\ lexyacc.cpp/\ =/ | grep -v \^\# >> $(CVS_DEPENDS)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules

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
	bison -d -v -t chapel.y
	cat chapel.tab.c | sed s/__attribute__/\\/\\// > chapel.tab.c.tmp
	mv chapel.tab.c.tmp chapel.tab.c
#	@if [`grep return chapel.y | wc -l` -ne 0 ]; then echo "PROBLEM: chapel.y contains return"; grep zyxv chapel.y; fi;


FORCE:

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot


$(PLATFORM)/chapel.tab.o: chapel.tab.c $(PLATFORM_DIR) $(LEXYACC_INCLUDES)
	$(CXX) -c $(CFLAGS) -Wno-error -o $@ $<

$(PLATFORM)/lex.yy.o: lex.yy.c $(PLATFORM_DIR) chapel.tab.h $(LEXYACC_INCLUDES)
	$(CXX) -c $(CFLAGS) -o $@ $<

$(PLATFORM)/yy.o: yy.c $(PLATFORM_DIR)
	$(CC) -c $(CFLAGS) -o $@ $<
