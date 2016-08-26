# Makefile for Chapel documentation

# Makefile.sphinx interfaces with sphinx build commands
include Makefile.sphinx

help: help-sphinx help-source

help-source:
	@echo "Source Help:"
	@echo "  docs           to invoke 'make html' in the virtualenv"
	@echo "  checkdocs      to invoke 'make check' in the virtualenv"
	@echo "  symlinks-docs  to symlink doc/release contents to source"
	@echo "  clean          to remove all generated files in source/"
	@echo "  clean-build    to remove all generated files in build/"
	@echo "  clobber        to remove all generated files in source/ and build/"
	@echo ""


docs: FORCE source
	./run-in-venv.bash $(MAKE) html

man-chapel: FORCE source
	./run-in-venv.bash $(MAKE) man


source: symlink-docs module-docs copy-docs
	@echo "Dynamically creating files in source/"


symlink-docs: clean-symlink-docs
	@echo "Symbolically linking docs from from doc/release to doc/sphinx/source"
	./symlinks.py

module-docs: clean-module-docs
	@echo "Generating module docs from \$CHPL_HOME/modules/ to doc/sphinx/source"
	(cd ../../modules && make documentation)

copy-docs: clean-copy-docs
	@echo "Copying files from doc/release to doc/sphinx/source/"
	cp  ../release/quickReference.pdf     source/language/quickReference.pdf
	cp  ../release/chapelLanguageSpec.pdf source/language/chapelLanguageSpec.pdf


checkdocs: FORCE
	./run-in-venv.bash $(MAKE) check


clean: clean-source

cleanall: clean-source

clobber: clean-source clean-build

clean-source: clean-symlink-docs clean-module-docs clean-copy-docs
	@echo "Cleaning all dynamically created files in source/"


clean-symlink-docs: FORCE
	@echo "Removing all symbolic links created in source/"
	find source -type l -delete

clean-copy-docs: FORCE
	@echo "Removing files that were copied into source/"
	rm -f source/language/quickReference.pdf
	rm -f source/language/chapelLanguageSpec.pdf

clean-module-docs: FORCE
	@echo "Removing module docs that were generated into source/"
	rm -rf source/modules/dists
	rm -rf source/modules/internal
	rm -rf source/modules/layouts
	rm -rf source/modules/packages
	rm -rf source/modules/standard


FORCE:
