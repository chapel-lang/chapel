# Copyright 2015 Cray Inc.
# Other additional copyright holders may be indicated within.
# 
# The entirety of this work is licensed under the Apache License,
# Version 2.0 (the "License"); you may not use this file except
# in compliance with the License.
# 
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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

SRCS= ViewField.cxx DataModel.cxx Event.cxx InfoBar.cxx LocaleWin.cxx
HFILES= ViewField.h DataModel.h Event.h InfoBar.h LocaleWin.h

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
chplvis.o: chplvis.h ViewField.h DataModel.h Event.h InfoBar.h LocaleWin.h
ViewField.o: ViewField.h DataModel.h Event.h
DataModel.o: DataModel.h Event.h
InfoBar.o: InfoBar.h ViewField.h
LocaleWin.o: LocaleWin.h
