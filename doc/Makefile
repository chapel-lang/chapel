# Makefile for Chapel documentation

# Makefile.sphinx interfaces with sphinx build commands
include Makefile.sphinx

CHPL2RST     = ./util/run-in-venv.bash ./util/chpl2rst.py

CHPL_DOC_PRIMERS_DIR = release/primers/primers
CHPL2RSTOPTS = --output=rst --prefix=$(CHPL_DOC_PRIMERS_DIR) --link=master

CHPL_DOC_HELLO_DIR = release/examples/hellos
CHPL2RSTOPTS_HELLO = --output=rst --prefix=$(CHPL_DOC_HELLO_DIR) --link=master

help: help-sphinx help-source

help-source:
	@echo "Source Help:"
	@echo "  docs           to invoke 'make html' in the virtualenv"
	@echo "  checkdocs      to invoke 'make check' in the virtualenv"
	@echo "  module-docs    to invoke 'make documentation' in "'$$CHPL_HOME/modules'""
	@echo "  primers        to generate primer docs in release/primers/"
	@echo "  hellos         to generate hellos docs in release/primers/"
	@echo "  man            to symbolically link the man page into release/usingchapel"
	@echo "  clean          to remove all generated files in release/"
	@echo "  clean-build    to remove all generated files in build/"
	@echo "  clobber        to remove all generated files in release/ and build/"
	@echo ""


docs: FORCE
	./util/run-in-venv.bash $(MAKE) html

man-chapel: FORCE
	./util/run-in-venv.bash $(MAKE) man


source: module-docs primers hellos symlinks


module-docs: clean-module-docs
	@echo
	@echo "Generating module docs from "'$$CHPL_HOME'"/modules/ into "'$$CHPL_HOME'"/doc/release"
	(cd ../modules && make documentation)

primers: clean-primers
	@echo
	@echo "Generating primers from "'$$CHPL_HOME'"/test/release/examples to "'$$CHPL_HOME'"/doc/release/primers/primers"
	@#Note - this assumes that we are not in a release tar ball
	$(CHPL2RST) $(CHPL2RSTOPTS) ../test/release/examples/primers/*.chpl
	$(CHPL2RST) $(CHPL2RSTOPTS) ../test/release/examples/primers/*doc.chpl --codeblock
	cp ../test/release/examples/primers/chplvis/*.chpl release/primers/primers/

hellos: clean-hellos
	@echo
	@echo "Generating hellos from "'$$CHPL_HOME'"/test/release/examples to "'$$CHPL_HOME'"/doc/release/$(CHPL_DOC_HELLO_DIR)"
	@#Note - this assumes that we are not in a release tar ball
	$(CHPL2RST) $(CHPL2RSTOPTS_HELLO) ../test/release/examples/hello*.chpl

symlinks: clean-symlinks
	@echo
	@echo "Creating symlinks to release/"
	ln -s $$CHPL_HOME/man/chpl.rst release/usingchapel/man.rst
	ln -s $$CHPL_HOME/man/chpldoc.rst release/tools/chpldoc/man.rst
	ln -s $$CHPL_HOME/test/release/examples release/users-guide/base/examples
	ln -s $$CHPL_HOME/test/release/examples release/users-guide/taskpar/examples
	ln -s $$CHPL_HOME/test/release/examples release/users-guide/datapar/examples
	ln -s $$CHPL_HOME/test/release/examples release/users-guide/locality/examples

checkdocs: FORCE
	./util/run-in-venv.bash $(MAKE) check


clean: clean-source

cleanall: clean-source

clobber: clean-source clean-build

clean-source: clean-module-docs clean-primers

clean-module-docs: FORCE
	@echo
	@echo "Removing module docs that were generated into release/"
	rm -rf release/modules/dists
	rm -rf release/builtins/
	rm -rf release/modules/layouts
	rm -rf release/modules/packages
	rm -rf release/modules/standard

clean-primers: FORCE
	@echo
	@echo "Removing primers generated into $(CHPL_DOC_HELLO_DIR)"
	rm -rf $(CHPL_DOC_PRIMERS_DIR)

clean-hellos: FORCE
	@echo
	@echo "Removing hellos generated into $(CHPL_DOC_HELLO_DIR)"
	rm -rf $(CHPL_DOC_HELLO_DIR)

clean-symlinks: FORCE
	@echo
	@echo "Removing all symbolic links from release/"
	find release -type l -delete


FORCE:

# Disable parallel builds to prevent race conditions
.NOTPARALLEL:

