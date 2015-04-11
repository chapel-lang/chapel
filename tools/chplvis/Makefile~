# A make file for this demo tool

# FLTK config section

CXX= $(shell fltk-config --cxx)
CXXFLAGS= $(shell fltk-config --cxxflags) -Wall -I. -g

LDFLAGS= $(shell fltk-config --ldflags)
LIBS= $(shell fltk-config --libs)

CHPL_HOME= $(shell printenv CHPL_HOME)
CHPL_HOST_PLATFORM= $(shell printenv CHPL_HOST_PLATFORM)

# Suffix rule for compiling .cxx files
.SUFFIXES: .o .h .cxx
.cxx.o:
	$(CXX) -c $(CXXFLAGS) $<

SRCS= ViewField.cxx DataModel.cxx Event.cxx InfoBar.cxx
HFILES= ViewField.h DataModel.h Event.h InfoBar.h

GENSRCS= chplvis.cxx
GENH= chplvis.h

OFILES= $(SRCS:.cxx=.o) $(GENSRCS:.cxx=.o)

chplvis: $(GENSRCS) $(OFILES) 
	$(CXX) -o chplvis $(OFILES) $(LDFLAGS) $(LIBS)

chplvis.cxx chplvis.h: chplvis.fl
	fluid -c chplvis.fl

clean:
	rm -f $(GENSRCS) $(GENH) $(OFILES) *~

clobber: clean
	rm -f chplvis

install: chplvis
	cp chplvis $(CHPL_HOME)/bin/$(CHPL_HOST_PLATFORM)

# Dependencies
chplvis.o: chplvis.h ViewField.h DataModel.h Event.h
ViewField.o: ViewField.h DataModel.h Event.h
DataModel.o: DataModel.h Event.h
