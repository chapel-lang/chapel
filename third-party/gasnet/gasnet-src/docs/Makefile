#   $Source: bitbucket.org:berkeleylab/gasnet.git/docs/Makefile $
# Description: Makefile for GASNet documentation
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>

# The GASNet-1 specification sources no longer live in this repo.
#
# The current version of the GASNet-1 specification is located at:
#
#  PDF:   https://gasnet.lbl.gov/dist/docs/gasnet.pdf
#  HTML:  https://gasnet.lbl.gov/dist/docs/gasnet.html
#  Text:  https://gasnet.lbl.gov/dist/docs/gasnet.txt

#OUTPUTS = gasnet.pdf gasnet.txt gasnet.html

clean:

dist-hook: clean
	rm -f Makefile pshm-design.txt
	@: # Use "blessed" GASNet docs if specified
	@if test -n "$(GASNET_DOCS)"; then \
	   for f in $(OUTPUTS); do \
	     if test -f $(GASNET_DOCS)/$$f; then rm -f $$f; fi; \
	     cmd="cp -p $(GASNET_DOCS)/$$f . && chmod a=r $$f"; \
	     echo $$cmd; eval $$cmd || exit $$?; \
	   done; \
	 fi
	@for f in $(OUTPUTS); do \
	   if test -f $$f ; then \
	      md5sum $$f ; \
	   else \
	     echo "WARNING: $$f not found in source directory or GASNET_DOCS='$(GASNET_DOCS)' : spec document omitted" ; \
	   fi ; \
	 done

.PHONY: clean
