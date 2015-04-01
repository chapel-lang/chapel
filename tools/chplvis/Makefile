# A make file for this demo tool

# FLTK config section

CXX= $(shell fltk-config --cxx)
CXXFLAGS= $(shell fltk-config --cxxflags) -Wall -I.

LDFLAGS= $(shell fltk-config --ldflags)
LIBS= $(shell fltk-config --libs)

# Suffix rule for compiling .cxx files
.SUFFIXES: .o .h .cxx
.cxx.o:
	$(CXX) -c $(CXXFLAGS) $<

SRCS= ViewField.cxx DataModel.cxx
HFILES= ViewField.h DataModel.h

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

# Dependencies
chplvis.o: chplvis.h ViewField.h DataModel.h
ViewField.o: ViewField.h DataModel.h
DataModel.o: DataModel.h

