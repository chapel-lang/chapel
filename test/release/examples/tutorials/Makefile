
MAKEFLAGS = --no-print-directory

CHPL = chpl

TARGETS = \
	arrays \
	block2D \
	classes \
	fileIO \
	genericClasses \
	iterators \
	multilocale \
	multirealm \
	opaque \
	ranges \
	reductions \
	slices \
	sparse \
	taskParallel \
	timers \
	varargs \
	variables \

REALS = $(TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)

#
# compiling a multirealm (multi-architecture) program is a bit
# different than most -- the -srealmTypes flag is used to list the
# multiple architectures to target.  This determines the number of and
# types of realms for the program.  The number of locales per realm is
# then set on the executable command-line using -nl/--numLocales for a
# single-realm execution and --localesPerRealm='# # #' for a
# multi-realm execution.  Here, we set both realms to be the target
# platform for portability, but in practice each realm is typically a
# distinct architecture type.  Typically we also compile with the
# $CHPL_HOME/util/chpl-multirealm script in order to simplify the
# compilation for each target architecture.  See
# $CHPL_HOME/doc/technotes/README.multirealm for more details.
#
CHPL_TARGET_PLATFORM = $(shell $(CHPL_HOME)/util/chplenv/platform --target)
#
multirealm: multirealm.chpl
	$(CHPL) -o $@ $< -srealmTypes="$(CHPL_TARGET_PLATFORM) $(CHPL_TARGET_PLATFORM)"

%: %.chpl
	$(CHPL) -o $@ $<

FORCE:
