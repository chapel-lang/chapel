# Makefile for Chapel documentation

# Sphinx-generated Makefile with minor modification
include Makefile.sphinx

help: help-sphinx help-source

help-source:
	@echo "Source Help:"
	@echo "  symlinks-docs to symlink doc/release contents to source"

docs: html

symlink-docs:
# Set up symlinks between `/doc/release` and `/doc/sphinx/source`
	./symlinks.py

clean: clean-build clean-source

clean-symlink-docs: FORCE
# Remove symlinks between `/doc/release` and `/doc/sphinx/source`
	./symlinks.py --clean

clean-module-docs: FORCE
	rm -rf source/modules/standard

clean-source: clean-symlink-docs

FORCE:
