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
COPY_IF_DIFFERENT = $(CHPL_MAKE_PYTHON) $(CHPL_MAKE_HOME)/util/config/update-if-different --copy


CHPL_DOC_PRIMERS_BUILD_DIR = ../build/doc/primers
CHPL_DOC_PRIMERS_RST_DIR = $(SOURCEDIR)/primers
CHPL2RSTOPTS = --output=rst --prefix=$(CHPL_DOC_PRIMERS_BUILD_DIR) --link=$(WEB_DOC_DIR)$(WEB_DOC_VERSION)

CHPL_DOC_EXAMPLES_BUILD_DIR = ../build/doc/examples
CHPL_DOC_EXAMPLES_RST_DIR = $(SOURCEDIR)/examples

CHPL_DOC_HELLO_BUILD_DIR = $(CHPL_DOC_EXAMPLES_BUILD_DIR)/hellos
CHPL_DOC_HELLO_RST_DIR = $(CHPL_DOC_EXAMPLES_RST_DIR)/hellos
CHPL2RSTOPTS_HELLO = --output=rst --prefix=$(CHPL_DOC_HELLO_BUILD_DIR) --link=$(WEB_DOC_DIR)$(WEB_DOC_VERSION)

SPEC_BUILD_DIR = ../build/doc/language/spec
SPEC_RST_DIR = $(SOURCEDIR)/language/spec

help: help-sphinx help-source

help-source:
	@echo "Source Help:"
	@echo "  docs           to invoke 'make html'"
	@echo "  checkdocs      to invoke 'make check'"
	@echo "  man-chapel     to invoke 'make man'"
	@echo "  module-docs    to invoke 'make documentation' in ../modules"
	@echo "  primers        to generate primer docs in $(SOURCEDIR)/primers/"
	@echo "  examples       to generate hellos docs in $(SOURCEDIR)/examples/"
	@echo "  symlinks       to create symlinks from ../man and ../test"
	@echo "  clean          to remove all generated files excluding ../build/doc"
	@echo "  clean-build    to remove all generated files in ../build/doc"
	@echo "  clobber        to remove all generated files"
	@echo


docs: FORCE
	@$(MAKE) html-release || $(MAKE) error_docs


man-chapel: FORCE
	$(MAKE) man


source: collect-syntax module-docs primers examples symlinks


collect-syntax:
	@echo
	@echo "Collecting syntax from '$(SPEC_RST_DIR)' into '$(SPEC_RST_DIR)/syntax.rst'"
	@mkdir -p $(SPEC_BUILD_DIR)
	@touch $(SPEC_BUILD_DIR)/syntax.rst
	@./util/collect-syntax.py rst/language/spec --outdir $(SPEC_BUILD_DIR)
	@$(COPY_IF_DIFFERENT) "$(SPEC_BUILD_DIR)/syntax.rst" "$(SPEC_RST_DIR)/syntax.rst"

module-docs:
	@echo
	@echo "Generating module docs from "'$$CHPL_HOME'"/modules/ into $(SOURCEDIR)/modules"
	@# modules/Makefile will
	@#   store module docs in doc/rst/modules and doc/rst/builtins
	@#   including meta/modules/* and meta/builtins/*
	@(cd ../modules && $(MAKE) documentation)

primers:
	@echo
	@echo "Generating primers from "'$$CHPL_HOME'"/test/release/examples to $(SOURCEDIR)/primers"
	@rm -rf $(CHPL_DOC_PRIMERS_BUILD_DIR)
	@mkdir -p $(CHPL_DOC_PRIMERS_BUILD_DIR)
	@#Note - this assumes that we are not in a release tar ball
	@$(CHPL2RST) $(CHPL2RSTOPTS) ../test/release/examples/primers/*.chpl
	@$(CHPL2RST) $(CHPL2RSTOPTS) ../test/release/examples/primers/*doc.chpl --codeblock
	@mkdir -p $(CHPL_DOC_PRIMERS_RST_DIR)
	@cp -f $(SOURCEDIR)/meta/primers/* $(CHPL_DOC_PRIMERS_BUILD_DIR)
	@$(COPY_IF_DIFFERENT) $(CHPL_DOC_PRIMERS_BUILD_DIR) $(CHPL_DOC_PRIMERS_RST_DIR)


examples:
	@echo
	@echo "Generating hellos from "'$$CHPL_HOME'"/test/release/examples to $(SOURCEDIR)/examples"
	@rm -rf $(CHPL_DOC_EXAMPLES_BUILD_DIR)
	@mkdir -p $(CHPL_DOC_EXAMPLES_BUILD_DIR)
	@$(CHPL2RST) $(CHPL2RSTOPTS_HELLO) ../examples/hello*.chpl
	@cp -f $(SOURCEDIR)/meta/examples/* $(CHPL_DOC_EXAMPLES_BUILD_DIR)
	@mkdir -p $(CHPL_DOC_EXAMPLES_RST_DIR)
	@$(COPY_IF_DIFFERENT) $(CHPL_DOC_EXAMPLES_BUILD_DIR) $(CHPL_DOC_EXAMPLES_RST_DIR)

symlinks:
	@echo
	@echo "Creating symlinks"
	@if [ ! -e $(SOURCEDIR)/usingchapel/man.rst ]; then ln -s $$CHPL_HOME/man/chpl.rst $(SOURCEDIR)/usingchapel/man.rst; fi
	@if [ ! -e $(SOURCEDIR)/tools/chpldoc/man.rst ]; then ln -s $$CHPL_HOME/man/chpldoc.rst $(SOURCEDIR)/tools/chpldoc/man.rst; fi
	@if [ ! -e $(SOURCEDIR)/tools/chplvis/examples ]; then ln -s $$CHPL_HOME/test/release/examples $(SOURCEDIR)/tools/chplvis/examples; fi
	@if [ ! -e $(SOURCEDIR)/users-guide/base/examples ]; then ln -s $$CHPL_HOME/test/release/examples $(SOURCEDIR)/users-guide/base/examples; fi
	@if [ ! -e $(SOURCEDIR)/users-guide/taskpar/examples ]; then ln -s $$CHPL_HOME/test/release/examples $(SOURCEDIR)/users-guide/taskpar/examples; fi
	@if [ ! -e $(SOURCEDIR)/users-guide/datapar/examples ]; then ln -s $$CHPL_HOME/test/release/examples $(SOURCEDIR)/users-guide/datapar/examples; fi
	@if [ ! -e $(SOURCEDIR)/users-guide/locality/examples ]; then ln -s $$CHPL_HOME/test/release/examples $(SOURCEDIR)/users-guide/locality/examples; fi

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


clean: clean-source clean-build clean-build-dir

cleanall: clean-source clean-build clean-build-dir

clobber: clean-source clean-build clean-build-dir

clean-source: clean-module-docs clean-primers clean-examples clean-symlinks clean-collect-syntax

clean-build-dir: FORCE
	rm -rf ../build/doc

clean-collect-syntax: FORCE
	@echo
	@echo "Removing generated RST file for syntax productions"
	touch $(SPEC_RST_DIR)/syntax.rst && rm $(SPEC_RST_DIR)/syntax.rst

clean-module-docs: FORCE
	@echo
	@echo "Removing module docs that were generated"
	rm -rf $(SOURCEDIR)/modules/
	rm -rf $(SOURCEDIR)/builtins/

clean-primers: FORCE
	@echo
	@echo "Removing primers generated into $(CHPL_DOC_HELLO_DIR)"
	rm -rf $(SOURCEDIR)/primers/

clean-examples: FORCE
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

