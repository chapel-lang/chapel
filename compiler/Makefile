#
# Makefile: builds Chapel compiler
#

COMPILER_ROOT=.
COMPILER_SUBDIR = 

#
# include standard header for compiler
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head



GEYSA_CVS_SRCS = \
	arg.cpp \
	ast.cpp \
	ast_cf.cpp \
	builtin.cpp \
	codegen.cpp \
	cfg.cpp \
	cg.cpp \
	clone.cpp \
	driver.cpp \
	dump.cpp \
	fa.cpp \
	files.cpp \
	fun.cpp \
	grammar.cpp \
	graph.cpp \
	if1.cpp \
	list.cpp \
	log.cpp \
	map.cpp \
	misc.cpp \
	mysystem.cpp \
	parse.cpp \
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
CHPL_OBJS = $(CHPL_SRCS:%.cpp=$(PLATFORM)/%.o)


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
	  echo -n $(PLATFORM)/ >> $(CVS_DEPENDS); \
	  $(CCDEP) -MM -MG $(CFLAGS) $$file >> $(CVS_DEPENDS); \
	done

test: FORCE
	cd test && $(MAKE) test


#
# target-based rules
#

$(CHPL): $(PLATFORM)/$(CHPL)
	rm -f $@
	ln -s $< $@

$(PLATFORM)/$(CHPL): $(CHPL_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(CHPL_OBJS) $(LIBS)
	@echo "Updating TAGS..."
	@etags $(CHPL_SRCS) *.h


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


#
# non-obvious dependences
#

$(PLATFORM)/v.g.d_parser.o: prim_data.h
$(PLATFORM)/ast_cf.o: cast_code.cpp


#
# include standard footer for compiler
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
