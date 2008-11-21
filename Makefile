#
# top-level Chapel Makefile
#

CHAPEL_ROOT = .
include $(CHAPEL_ROOT)/make/Makefile.base

ifeq ($(CHPL_MAKE_PLATFORM), darwin)
USE_XCODEBUILD=yes
SYMROOT=/tmp 
endif

default: all

all: comprt man

ifeq ($(USE_XCODEBUILD), yes)
comprt:
	cd etc && xcodebuild SYMROOT=$(SYMROOT) ARCHS='$$NATIVE_ARCH'
else
comprt: compiler runtime
endif

compiler: FORCE
ifeq ($(USE_XCODEBUILD), yes)
	cd etc && xcodebuild -target Chapel SYMROOT=$(SYMROOT) ARCHS='$$NATIVE_ARCH'
else
	cd compiler && $(MAKE)
endif

man: FORCE
	@test -r man/Makefile && $(MAKE) manhelp || echo ""

manhelp: FORCE
	cd man && $(MAKE)

runtime: FORCE
ifeq ($(USE_XCODEBUILD), yes)
	cd etc && xcodebuild -target Runtime SYMROOT=$(SYMROOT) ARCHS='$$NATIVE_ARCH'
else
	cd runtime && $(MAKE)
endif

third-party: FORCE
	cd third-party && $(MAKE)

test: FORCE
	cd test && start_test

clean: FORCE
ifeq ($(USE_XCODEBUILD), yes)
	cd etc && xcodebuild clean SYMROOT=$(SYMROOT)
else
	cd compiler && $(MAKE) clean
	cd runtime && $(MAKE) clean
endif

clobber: FORCE
	cd compiler && $(MAKE) clobber
	cd runtime && $(MAKE) clobber
	rm -rf bin
	rm -rf lib

depend: FORCE
	cd compiler && $(MAKE) depend
	cd runtime && $(MAKE) depend

FORCE:
