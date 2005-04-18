#
# Makefile: builds Chapel compiler
#

COMPILER_ROOT=.
COMPILER_SUBDIR = .

USE_DPARSER=1


#
# subdirectories and false subdirectory-oriented targets to force recursion
#
SUBDIRS = \
	AST \
	adt \
	analysis \
	backend \
	main \
	parser \
	passes \
	symtab \
	test \
	traversals \
	util \
	vparser \

#
# include standard header for compiler
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head


#
# include source subdirectories here
#
include AST/Makefile.include
include adt/Makefile.include
include analysis/Makefile.include
include backend/Makefile.include
include main/Makefile.include
include parser/Makefile.include
include passes/Makefile.include
include symtab/Makefile.include
include traversals/Makefile.include
include util/Makefile.include
include vparser/Makefile.include

CVS_SRCS = 

CHPL = chpl$(CHPL_MUNGE)$(EXE_SUFFIX)
CHPL_OBJS = \
	$(AST_OBJS) \
	$(ADT_OBJS) \
	$(ANALYSIS_OBJS) \
	$(BACKEND_OBJS) \
	$(MAIN_OBJS) \
	$(PARSER_OBJS) \
	$(PASSES_OBJS) \
	$(SYMTAB_OBJS) \
	$(TRAVERSALS_OBJS) \
	$(UTIL_OBJS) \
	$(VPARSER_OBJS) \

EXECS = $(CHPL)

TARGETS = $(CHPL)


#
# main rules
#

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules


#
# target-based rules
#

$(CHPL): $(PLATFORM)/$(CHPL)
	cp -f $< $@

$(PLATFORM)/$(CHPL): $(PLATFORM_DIR) $(CHPL_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(CHPL_OBJS) $(LIBS)
	@echo "Updating TAGS..."
	@etags 	$(D_PARSER_SRC_DIR)/*.h $(D_PARSER_SRC_DIR)/*.c \
		$(GC_SRC_DIR)/*.h $(GC_SRC_DIR)/*.c \
		$(GC_SRC_DIR)/*/*.h $(GC_SRC_DIR)/*/*.c \
		$(ALL_SRCS) */*.h
	-@(which ebrowse > /dev/null 2>&1 && echo "Updating BROWSE..." && ebrowse $(ALL_SRCS) */*.h) || echo "ebrowse not available"


#
# include standard footer for compiler
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
