## Process this file with automake to produce Makefile.in	-*- makefile -*-

AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = config.guess config.sub depcomp install-sh missing mkinstalldirs

distclean-local:
	rm -f Makefile

# These appear here to enforce check-prefix-vars early in the top-level recursive install traversal
install-data-local: 
	@$(MAKE) -C ..  check-prefix-vars

install-exec-local: 
	@$(MAKE) -C ..  check-prefix-vars
