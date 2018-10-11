CHPL = chpl

#
# Once you have a working program in Chapel and you want to start
# doing performance studies, throw the following flag.  It disables
# runtime checks, enables optimization of the generated code, etc.
# Whenever you make significant changes to your code, remove this flag
# to turn runtime checks back on again.
#
# COMPOPTS = --fast
#

all: c-ray

c-ray: c-ray.chpl
	$(CHPL) $(COMPOPTS) $<
