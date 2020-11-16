ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/..
endif

ifndef CHPL_MAKE_PYTHON
export CHPL_MAKE_PYTHON := $(shell $(CHPL_MAKE_HOME)/util/config/find-python.sh)
endif

# Makefile for Chapel documentation

# for a release, change the following to 'release/' and 'x.yz' (the
# release's verion number
#
WEB_DOC_DIR=
WEB_DOC_VERSION=master

# Makefile.sphinx interfaces with sphinx build commands
include Makefile.sphinx

CHPLDEPS = $(shell $(CHPL_MAKE_PYTHON) $(CHPL_MAKE_HOME)/util/chplenv/chpl_home_utils.py --chpldeps)

CHPL2RST = $(CHPL_MAKE_PYTHON) $(CHPLDEPS) ./util/chpl2rst.py

CHPL_DOC_PRIMERS_DIR = $(SOURCEDIR)/primers
CHPL2RSTOPTS = --output=rst --prefix=$(CHPL_DOC_PRIMERS_DIR) --link=$(WEB_DOC_DIR)$(WEB_DOC_VERSION)

CHPL_DOC_HELLO_DIR = $(SOURCEDIR)/examples/hellos
CHPL2RSTOPTS_HELLO = --output=rst --prefix=$(CHPL_DOC_HELLO_DIR) --link=$(WEB_DOC_DIR)$(WEB_DOC_VERSION)

SPECSRCDIR = $(SOURCEDIR)/language/spec

help: help-sphinx help-source

help-source:
	@echo "Source Help:"
	@echo "  docs           to invoke 'make html'"
	@echo "  checkdocs      to invoke 'make check'"
	@echo "  man-chapel     to invoke 'make man'"
	@echo "  module-docs    to invoke 'make documentation' in ../modules"
	@echo "  primers        to generate primer docs in $(SOURCEDIR)/primers/"
	@echo "  hellos         to generate hellos docs in $(SOURCEDIR)/examples/"
	@echo "  symlinks       to create symlinks from ../man and ../test"
	@echo "  clean          to remove all generated files excluding ../build/doc"
	@echo "  clean-build    to remove all generated files in ../build/doc"
	@echo "  clobber        to remove all generated files"
	@echo


docs: FORCE
	@$(MAKE) html-release || $(MAKE) error_docs


man-chapel: FORCE
	$(MAKE) man


source: collect-syntax module-docs primers hellos symlinks


collect-syntax: clean-collect-syntax
	@echo
	@echo "Collecting syntax from '$(SPECSRCDIR)' into '$(SPECSRCDIR)/syntax.rst'"
	./util/collect-syntax.py rst/language/spec --outdir $(SPECSRCDIR)

module-docs: clean-module-docs
	@echo
	@echo "Generating module docs from "'$$CHPL_HOME'"/modules/ into $(SOURCEDIR)/modules"
	(cd ../modules && $(MAKE) documentation)
	cp -f $(SOURCEDIR)/meta/modules/* $(SOURCEDIR)/modules/
	cp -f $(SOURCEDIR)/meta/builtins/* $(SOURCEDIR)/builtins/

primers: clean-primers
	@echo
	@echo "Generating primers from "'$$CHPL_HOME'"/test/release/examples to $(SOURCEDIR)/primers"
	@#Note - this assumes that we are not in a release tar ball
	$(CHPL2RST) $(CHPL2RSTOPTS) ../test/release/examples/primers/*.chpl
	$(CHPL2RST) $(CHPL2RSTOPTS) ../test/release/examples/primers/*doc.chpl --codeblock
	cp -f $(SOURCEDIR)/meta/primers/* $(SOURCEDIR)/primers

hellos: clean-hellos
	@echo
	@echo "Generating hellos from "'$$CHPL_HOME'"/test/release/examples to $(SOURCEDIR)/examples"
	$(CHPL2RST) $(CHPL2RSTOPTS_HELLO) ../examples/hello*.chpl
	cp -f $(SOURCEDIR)/meta/examples/* $(SOURCEDIR)/examples

symlinks: clean-symlinks
	@echo
	@echo "Creating symlinks"
	ln -s $$CHPL_HOME/man/chpl.rst $(SOURCEDIR)/usingchapel/man.rst
	ln -s $$CHPL_HOME/man/chpldoc.rst $(SOURCEDIR)/tools/chpldoc/man.rst
	ln -s $$CHPL_HOME/test/release/examples $(SOURCEDIR)/tools/chplvis/examples
	ln -s $$CHPL_HOME/test/release/examples $(SOURCEDIR)/users-guide/base/examples
	ln -s $$CHPL_HOME/test/release/examples $(SOURCEDIR)/users-guide/taskpar/examples
	ln -s $$CHPL_HOME/test/release/examples $(SOURCEDIR)/users-guide/datapar/examples
	ln -s $$CHPL_HOME/test/release/examples $(SOURCEDIR)/users-guide/locality/examples

checkdocs: FORCE
	$(MAKE) check

%.docs: FORCE
	$(MAKE) $*.html

prunedocs: FORCE
	rm -f html/_downloads/quickReference.pdf
	rm -f html/_downloads/chapelLanguageSpec.pdf
	if [ -d "html" ]; then \
	ln -s ../../$(SOURCEDIR)/language/quickReference.pdf html/_downloads/quickReference.pdf; \
	ln -s ../../$(SOURCEDIR)/language/chapelLanguageSpec.pdf html/_downloads/chapelLanguageSpec.pdf; \
	fi


clean: clean-source

cleanall: clean-source

clobber: clean-source clean-build

clean-source: clean-module-docs clean-primers clean-hellos clean-symlinks

clean-collect-syntax: FORCE
	@echo
	@echo "Removing generated RST file for syntax productions"
	touch $(SPECSRCDIR)/syntax.rst && rm $(SPECSRCDIR)/syntax.rst

clean-module-docs: FORCE
	@echo
	@echo "Removing module docs that were generated"
	rm -rf $(SOURCEDIR)/modules/
	rm -rf $(SOURCEDIR)/builtins/

clean-primers: FORCE
	@echo
	@echo "Removing primers generated into $(CHPL_DOC_HELLO_DIR)"
	rm -rf $(SOURCEDIR)/primers/

clean-hellos: FORCE
	@echo
	@echo "Removing hellos generated into $(CHPL_DOC_HELLO_DIR)"
	rm -rf $(SOURCEDIR)/examples/

clean-symlinks: FORCE
	@echo
	@echo "Removing all symbolic links"
	find $(SOURCEDIR) -type l -delete

error_docs: FORCE
	@exit 1 # Note that 'make' will return exit code of 2, despite exit 1


FORCE:

# Disable parallel builds to prevent race conditions
.NOTPARALLEL:

