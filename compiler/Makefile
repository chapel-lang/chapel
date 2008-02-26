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
	optimizations \
	parser \
	passes \
	util \

#
# include standard header for compiler
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

#
# the location of the BUILD_VERSION file
#
BUILD_VERSION_FILE = $(COMPILER_ROOT)/main/BUILD_VERSION



#
# include source subdirectories here
#
include adt/Makefile.include
include AST/Makefile.include
include backend/Makefile.include
include ifa/Makefile.include
include main/Makefile.include
include optimizations/Makefile.include
include parser/Makefile.include
include passes/Makefile.include
include util/Makefile.include

SVN_SRCS = 

CHPL_BIN_DIR = ../bin/$(CHPL_MAKE_PLATFORM)
CHPL = $(CHPL_BIN_DIR)/chpl$(EXE_SUFFIX)
CHPL_OBJS = \
	$(ADT_OBJS) \
	$(AST_OBJS) \
	$(BACKEND_OBJS) \
	$(IFA_OBJS) \
	$(MAIN_OBJS) \
	$(OPTIMIZATIONS_OBJS) \
	$(PARSER_OBJS) \
	$(PASSES_OBJS) \
	$(SYMTAB_OBJS) \
	$(UTIL_OBJS) \

EXECS = $(CHPL)

PRETARGETS = $(BUILD_VERSION_FILE)
TARGETS = $(CHPL)


#
# main rules
#

include $(COMPILER_ROOT)/make/Makefile.compiler.rules


#
# target-based rules
#

$(BUILD_VERSION_FILE): FORCE
	@$(CHAPEL_ROOT)/util/updateBuildVersion.pl $(CHAPEL_ROOT)

$(CHPL): $(CHPL_BIN_DIR) $(CHPL_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(CHPL_OBJS) $(LIBS)
	-@(which etags > /dev/null 2>&1 && echo "Updating TAGS..." && etags $(ALL_SRCS) */*.h) || echo "etags not available"
	-@(which ebrowse > /dev/null 2>&1 && echo "Updating BROWSE..." && ebrowse $(ALL_SRCS) */*.h) || echo "ebrowse not available"

$(CHPL_BIN_DIR):
	mkdir -p $@



#
# include standard footer for compiler
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
