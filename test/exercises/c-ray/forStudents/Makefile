CHPL = chpl

#
# Once you have a working program in Chapel and you want to start
# doing performance studies, throw the following flag.  It disables
# runtime checks, enables optimization of the generated code, etc.
#
# COMPOPTS = --fast
#

all: c-ray

c-ray: c-ray.chpl
	$(CHPL) -o $@ $(COMPOPTS) $<
