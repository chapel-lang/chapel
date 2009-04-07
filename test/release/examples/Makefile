
MAKEFLAGS = --no-print-directory

CHPL = chpl

TARGETS = \
	hello \
	hello-module \
	hello-multiloc \


default: all tutorials programs hpcc 

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS)
	cd tutorials && $(MAKE) clean
	cd programs && $(MAKE) clean
	cd hpcc && $(MAKE) clean

tutorials: FORCE
	cd tutorials && $(MAKE) all

programs: FORCE
	cd programs && $(MAKE) all

hpcc: FORCE
	cd hpcc && $(MAKE) all

%: %.chpl
	$(CHPL) -o $@ $<

FORCE:
