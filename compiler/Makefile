#
# Makefile: builds Chapel compiler
#

COMPILER_ROOT=.
COMPILER_SUBDIR = .



#
# subdirectories and false subdirectory-oriented targets to force recursion
#
SUBDIRS = \
	adt \
	AST \
	backend \
	ifa \
	main \
	parser \
	passes \
	util \

#
# include standard header for compiler
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head


#
# include source subdirectories here
#
include adt/Makefile.include
include AST/Makefile.include
include backend/Makefile.include
include ifa/Makefile.include
include main/Makefile.include
include parser/Makefile.include
include passes/Makefile.include
include util/Makefile.include

CVS_SRCS = 

CHPL_BIN_DIR = ../bin/$(PLATFORM)
CHPL = $(CHPL_BIN_DIR)/chpl$(EXE_SUFFIX)
CHPL_OBJS = \
	$(ADT_OBJS) \
	$(AST_OBJS) \
	$(BACKEND_OBJS) \
	$(IFA_OBJS) \
	$(MAIN_OBJS) \
	$(PARSER_OBJS) \
	$(PASSES_OBJS) \
	$(SYMTAB_OBJS) \
	$(UTIL_OBJS) \

EXECS = $(CHPL)

TARGETS = $(CHPL)


#
# main rules
#

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules


#
# target-based rules
#

$(CHPL): $(CHPL_BIN_DIR) $(CHPL_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(CHPL_OBJS) $(LIBS)
	@echo "Updating TAGS..."
	@etags $(ALL_SRCS) */*.h
	-@(which ebrowse > /dev/null 2>&1 && echo "Updating BROWSE..." && ebrowse $(ALL_SRCS) */*.h) || echo "ebrowse not available"

$(CHPL_BIN_DIR):
	mkdir -p $@



#
# include standard footer for compiler
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
