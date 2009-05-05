#*****************************************************************************#
# 
# makefile
# for PVM in the Cygwin environement
#
#*****************************************************************************#

intro:
	@echo
	@echo "This is a makefile for (re)compiling the PVM code" 
	@echo "and/or the examples for the cygwin environment. For"
	@echo "generic use, you must provide one of the following inputs:"
	@echo 
	@echo "make -f cygwin.mak pvm"
	@echo "         --      will recompile and reinstall the PVM code"
	@echo "                 by compiling the libpvm, daemon,"
	@echo "                 groupserver and console"
	@echo
	@echo "make -f cygwin.mak examples"
	@echo "         --      will recompile the C examples"
	@echo
	@echo "make -f cygwin.mak all"
	@echo "         --      will compile the items above"
	@echo
	@echo "make -f cygwin.mak clean"
	@echo "         --      will erase the object files"
	@echo
	@echo "make -f cygwin.mak tidy"
	@echo "         --      will erase the object and executables"
	@echo

all: pvm examples

pvm: _lib _tracer _console _pvmgs
	@echo "Installation complete."
	@echo

_lib:
	@echo "Compiling and installing library (C) and daemon" 
	cd src ; \
	 make -f cygwin.mak
	@echo
	touch _lib

_tracer:
	@echo "Compiling and installing tracer"
	cd tracer ; \
	 make -f cygwin.mak
	@echo
	touch _tracer

_console:
	@echo "Compiling and installing console"
	cd console ; \
	 make -f cygwin.mak
	@echo
	touch _console

_pvmgs:
	@echo "Compiling and installing groupserver"
	cd pvmgs ; \
	 make -f cygwin.mak
	@echo
	touch _pvmgs

examples: _cexamples

_cexamples:
	@echo "Building C examples"
	cd examples ; \
	 make -f cygwin.mak c-all
	@echo
	touch _cexamples

tidy: clean
	@echo "Cleaning Execs and Libs ..."
	rm -f bin/win32/*.exe
	rm -f console/win32/*.exe
	rm -f tracer/win32/*.{exe,a}
	rm -f lib/win32/*.{exe,a}
	@echo

clean:
	@echo "Cleaning objects ..."
	rm -f _lib src/win32/*.o
	@echo "Cleaning console objects ..."
	rm -f _console console/win32/*.o
	@echo "Cleaning pvmgs objects ..."
	rm -f _pvmgs pvmgs/win32/*.o
	@echo "Cleaning tracer objects ..."
	rm -f _tracer tracer/win32/*.o
	@echo "Cleaning examples objects ..."
	rm -f _cexamples examples/win32/*.o
	@echo

