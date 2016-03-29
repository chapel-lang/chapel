# Makefile for Chapel documentation

# Sphinx-generated Makefile with minor modification
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

docs: FORCE
	./run-in-venv.bash $(MAKE) html

man-chapel: FORCE
	./run-in-venv.bash $(MAKE) man

symlink-docs: clean-symlink-docs
	@echo "Setting up symlinks from /doc/release to /doc/sphinx/source"
	./symlinks.py
	cp  ../release/quickReference.pdf     source/language/quickReference.pdf
	cp  ../release/chapelLanguageSpec.pdf source/language/chapelLanguageSpec.pdf

checkdocs: FORCE
	./run-in-venv.bash $(MAKE) check

clean: clean-source

cleanall: clean-source

clobber: clean-source clean-build

clean-source: clean-symlink-docs clean-module-docs

clean-symlink-docs: FORCE
	@echo "Removing all symbolic links contained in source/"
	find source -type l -delete
	rm -f language/quickReference.pdf
	rm -f language/chapelLanguageSpec.pdf

clean-module-docs: FORCE
	rm -rf source/modules/dists
	rm -rf source/modules/internal
	rm -rf source/modules/layouts
	rm -rf source/modules/packages
	rm -rf source/modules/standard

FORCE:
