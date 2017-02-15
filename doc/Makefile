# Makefile for Chapel documentation

# Makefile.sphinx interfaces with sphinx build commands
include Makefile.sphinx

CHPL2RST     = ../util/run-in-venv.bash ./util/chpl2rst.py

CHPL_DOC_PRIMERS_DIR = primers/primers
CHPL2RSTOPTS = --output=rst --prefix=$(CHPL_DOC_PRIMERS_DIR) --link=master

CHPL_DOC_HELLO_DIR = examples/hellos
CHPL2RSTOPTS_HELLO = --output=rst --prefix=$(CHPL_DOC_HELLO_DIR) --link=master

help: help-sphinx help-source

help-source:
	@echo "Source Help:"
	@echo "  docs           to invoke 'make html' in the virtualenv"
	@echo "  checkdocs      to invoke 'make check' in the virtualenv"
	@echo "  module-docs    to invoke 'make documentation' in ../modules"
	@echo "  primers        to generate primer docs in primers/"
	@echo "  hellos         to generate hellos docs in primers/"
	@echo "  symlinks       to create symlinks from ../man and ../test"
	@echo "  clean          to remove all generated files excluding ../build/doc"
	@echo "  clean-build    to remove all generated files in ../build/doc"
	@echo "  clobber        to remove all generated files"
	@echo


docs: FORCE
	@../util/run-in-venv.bash $(MAKE) html-release || (echo;echo "If a documentation hierarchy change was made on the master branch, \
	try resolving with: \`git clean "'$$CHPL_HOME'"/doc/*\`";echo)

man-chapel: FORCE
	../util/run-in-venv.bash $(MAKE) man


source: module-docs primers hellos symlinks


module-docs: clean-module-docs
	@echo
	@echo "Generating module docs from "'$$CHPL_HOME'"/modules/ into "'$$CHPL_HOME'"/doc/"
	(cd ../modules && make documentation)
	cp -f meta/modules/* modules/
	cp -f meta/builtins/* builtins/

primers: clean-primers
	@echo
	@echo "Generating primers from "'$$CHPL_HOME'"/test/release/examples to "'$$CHPL_HOME'"/doc/primers/primers"
	@#Note - this assumes that we are not in a release tar ball
	$(CHPL2RST) $(CHPL2RSTOPTS) ../test/release/examples/primers/*.chpl
	$(CHPL2RST) $(CHPL2RSTOPTS) ../test/release/examples/primers/*doc.chpl --codeblock
	cp -f meta/primers/* primers

hellos: clean-hellos
	@echo
	@echo "Generating hellos from "'$$CHPL_HOME'"/test/release/examples to "'$$CHPL_HOME'"/doc/$(CHPL_DOC_HELLO_DIR)"
	@#Note - this assumes that we are not in a release tar ball
	$(CHPL2RST) $(CHPL2RSTOPTS_HELLO) ../test/release/examples/hello*.chpl
	cp -f meta/examples/* examples

symlinks: clean-symlinks
	@echo
	@echo "Creating symlinks"
	ln -s $$CHPL_HOME/man/chpl.rst usingchapel/man.rst
	ln -s $$CHPL_HOME/man/chpldoc.rst tools/chpldoc/man.rst
	ln -s $$CHPL_HOME/test/release/examples tools/chplvis/examples
	ln -s $$CHPL_HOME/test/release/examples users-guide/base/examples
	ln -s $$CHPL_HOME/test/release/examples users-guide/taskpar/examples
	ln -s $$CHPL_HOME/test/release/examples users-guide/datapar/examples
	ln -s $$CHPL_HOME/test/release/examples users-guide/locality/examples

checkdocs: FORCE
	../util/run-in-venv.bash $(MAKE) check

%.docs: FORCE
	../util/run-in-venv.bash $(MAKE) $*.html


clean: clean-source

cleanall: clean-source

clobber: clean-source clean-build

clean-source: clean-module-docs clean-primers clean-hellos clean-symlinks

clean-module-docs: FORCE
	@echo
	@echo "Removing module docs that were generated"
	rm -rf modules/
	rm -rf builtins/

clean-primers: FORCE
	@echo
	@echo "Removing primers generated into $(CHPL_DOC_HELLO_DIR)"
	rm -rf primers/

clean-hellos: FORCE
	@echo
	@echo "Removing hellos generated into $(CHPL_DOC_HELLO_DIR)"
	rm -rf examples/

clean-symlinks: FORCE
	@echo
	@echo "Removing all symbolic links"
	find . -type l -delete


FORCE:

# Disable parallel builds to prevent race conditions
.NOTPARALLEL:

