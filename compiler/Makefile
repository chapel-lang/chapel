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
	traversals \
	util \
	vparser \

#	cast \
#	make_prims \
#	test \

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
include traversals/Makefile.include
include util/Makefile.include
include vparser/Makefile.include


CHPL = chpl$(EXE_SUFFIX)
CHPL_OBJS = \
	$(AST_OBJS) \
	$(ADT_OBJS) \
	$(ANALYSIS_OBJS) \
	$(BACKEND_OBJS) \
	$(MAIN_OBJS) \
	$(PARSER_OBJS) \
	$(PASSES_OBJS) \
	$(TRAVERSALS_OBJS) \
	$(UTIL_OBJS) \
	$(VPARSER_OBJS) \

EXECS = $(CHPL)


#
# main rules
#

all: $(MAKEALLSUBDIRS) echocompilerdir $(CHPL)

clean: $(CLEANSUBDIRS) echocompilerdir
	rm -f $(CLEAN_TARGS)

cleandeps: $(CLEANSUBDIRDEPS) echocompilerdir
	rm -f $(CVS_DEPENDS)

clobber: $(CLOBBERSUBDIRS) echocompilerdir
	rm -rf $(CLOBBER_TARGS)

depend: $(DEPENDSUBDIRS) $(PLATFORM_DIR)

test: FORCE
	cd test && $(MAKE) test


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
		$(ALL_SRCS)
	@echo "Updating BROWSE..."
	@ebrowse $(ALL_SRCS)


#
# include standard footer for compiler
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
