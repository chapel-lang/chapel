#
# Makefile: builds Chapel compiler
#

COMPILER_ROOT=.
COMPILER_SUBDIR = 

#
# include standard header for compiler
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

#
# include source subdirectories here
#
include AST/Makefile.include


GEYSA_CVS_SRCS = \
	analysis.cpp \
	arg.cpp \
	ast.cpp \
	ast_cf.cpp \
	beautify.cpp \
	builtin.cpp \
	cdb.cpp \
	cfg.cpp \
	cg.cpp \
	cg_funs.cpp \
	clone.cpp \
	codegen.cpp \
	createAST.cpp \
	datatype.cpp \
	dom.cpp \
	driver.cpp \
	dump.cpp \
	fa.cpp \
	files.cpp \
	fun.cpp \
	grammar.cpp \
	graph.cpp \
	if1.cpp \
	inline.cpp \
	list.cpp \
	log.cpp \
	loop.cpp \
	map.cpp \
	misc.cpp \
	mysystem.cpp \
	parse.cpp \
	parse_ast.cpp \
	pattern.cpp \
	pdb.cpp \
	pnode.cpp \
	prim.cpp \
	region.cpp \
	scope.cpp \
	ssu.cpp \
	stringutil.cpp \
	var.cpp \
	vec.cpp \
	version.cpp
GEYSA_GEN_SRCS = \
	$(LANGS:%=%.g.d_parser.cpp) \
	prim_data.cpp

CHPL = chpl$(EXE_SUFFIX)
CHPL_SRCS = $(GEYSA_GEN_SRCS) $(GEYSA_CVS_SRCS)
CHPL_OBJS = \
	$(CHPL_SRCS:%.cpp=$(PLATFORM)/%.o) \
	$(AST_OBJS)


#
# standard variables
#
CVS_SRCS = $(GEYSA_CVS_SRCS)
GEN_SRCS = $(GEYSA_GEN_SRCS) cast_code.cpp prim_data.h
EXECS = $(CHPL)


#
# subdirectories and false subdirectory-oriented targets to force recursion
#
SUBDIRS = \
	AST \
	cast \
	make_prims \
	test
MAKEALLSUBDIRS = $(SUBDIRS:%=%.makedir)
CLEANSUBDIRS = $(SUBDIRS:%=%.cleandir)
CLEANSUBDIRDEPS = $(SUBDIRS:%=%.cleandirdeps)
CLOBBERSUBDIRS = $(SUBDIRS:%=%.clobberdir)
DEPENDSUBDIRS = $(SUBDIRS:%=%.dependdir)



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

depend: $(DEPENDSUBDIRS) echocompilerdir $(PLATFORM_DIR)
	@echo Updating dependences...
	@echo > $(CVS_DEPENDS)
	@for file in $(CVS_SRCS); do \
	  $(ECHO) -n $(PLATFORM)/ >> $(CVS_DEPENDS); \
	  $(CXX) -MM -MG $(CXXFLAGS) $$file | grep -v ^# >> $(CVS_DEPENDS); \
	done
	@$(CXX) -MM -MG -MT PARSE_INCLUDES $(CFLAGS) parse_incs.cpp | sed s/:\ parse_incs.cpp/\ =/ | grep -v ^# >> $(CVS_DEPENDS)
	@$(CXX) -MM -MG -MT PRIM_DATA_INCLUDES $(CFLAGS) prim_data_incs.cpp | sed s/:\ prim_data_incs.cpp/\ =/ | grep -v ^# >> $(CVS_DEPENDS)

test: FORCE
	cd test && $(MAKE) test


#
# target-based rules
#

$(CHPL): $(PLATFORM)/$(CHPL)
	rm -f $@
	ln -s $< $@

$(PLATFORM)/$(CHPL): $(CHPL_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(CHPL_OBJS) $(LIBS)
	@echo "Updating TAGS..."
	@etags $(CHPL_SRCS) *.h \
		$(D_PARSER_SRC_DIR)/*.c $(D_PARSER_SRC_DIR)/*.h \
		$(GC_SRC_DIR)/*.c $(GC_SRC_DIR)/*.h \
		$(GC_SRC_DIR)/*/*.c $(GC_SRC_DIR)/*/*.h \
		AST/*.h AST/*.cpp


#
# how to generate sources that don't already exist
#

CAST = cast/cast$(EXE_SUFFIX)
MAKE_PRIMS = ./make_prims/make_prims$(EXE_SUFFIX)

cast_code.cpp: $(CAST)
	./$(CAST)

prim_data.h prim_data.cpp: $(MAKE_PRIMS) prim_data.dat
	$(MAKE_PRIMS) prim_data.dat



#
# recursive subdirectory rules
#

STARS = \*\*\*\*\*

echocompilerdir:
	@echo $(STARS) compiler/. $(STARS)

%.makedir:
	@echo $(STARS) compiler/$* $(STARS)
	@cd $* && $(MAKE)

%.cleandir:
	@echo $(STARS) compiler/$* $(STARS)
	@cd $* && $(MAKE) clean

%.cleandirdeps:
	@echo $(STARS) compiler/$* $(STARS)
	@cd $* && $(MAKE) cleandeps

%.clobberdir:
	@echo $(STARS) compiler/$* $(STARS)
	@cd $* && $(MAKE) clobber

%.dependdir:
	@echo $(STARS) compiler/$* $(STARS)
	@cd $* && $(MAKE) depend


# Here, prim_data.h needs to be mentioned explicitly to get
# prim_data.h created before we try to create %.g.d_parser.cpp, just
# in case the user hasn't already run a "make depend" (in which case
# GEYSA_INCLUDES will be set and the dependences in the generated file
# will capture this information redundantly).

$(PLATFORM)/v.g.d_parser.o: prim_data.h
$(PLATFORM)/chpl.g.d_parser.o: prim_data.h


#
# include standard footer for compiler
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot

#
# non-obvious dependences -- these must follow
# Makefile.compiler.foot (technically, Makefile.cvs.depend,
# which it includes) due to the use of the
# *_INCLUDES variables...
#

$(PLATFORM)/ast_cf.o: cast_code.cpp
$(PLATFORM)/prim_data.o: $(PRIM_DATA_INCLUDES)
$(PLATFORM)/%.g.d_parser.o: $(PARSE_INCLUDES)