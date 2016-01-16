# Chapel Makefile

# TODO Find a way to make this fit into the existing Makefile system

OPTFLAGS = --fast -sassertNoSlicing
MODULEFLAGS = --module-dir ../../include/

# Debugging
DEBUGFLAGS =
ifdef DEBUG
DEBUGFLAGS = --savec c
endif

CHPL = chpl

SOURCES = p2p-serial.chpl
EXECUTABLE = $(SOURCES:.chpl=)

all: $(EXECUTABLE)

%: %.chpl
	$(CHPL) $(OPTFLAGS) $(MODULEFLAGS) $(DEBUGFLAGS) $*.chpl -o $*

clean:
	rm -f $(EXECUTABLE)
