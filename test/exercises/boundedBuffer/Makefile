CHPL = chpl

#
# Once you have a working program in Chapel and you want to start
# doing performance studies, throw the following flag.  It disables
# runtime checks, enables optimization of the generated code, etc.
#
# COMPOPTS = --fast
#

all: boundedBuffer

boundedBuffer: boundedBuffer.chpl
	$(CHPL) -o $@ $(COMPOPTS) $<
