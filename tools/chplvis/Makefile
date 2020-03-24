# Copyright 2020 Hewlett Packard Enterprise Development LP
# Copyright 2015-2019 Cray Inc.
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

# chplvis -- a tool to visualize communication and tasks across locales.

# Chapel config

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

# FLTK config section

FLTK_CONFIG=$(FLTK_INSTALL_DIR)/bin/fltk-config
FLTK_FLUID=$(FLTK_INSTALL_DIR)/bin/fluid

CXXFLAGS=  -Wall -I. -g

# Suffix rule for compiling .cxx files
.SUFFIXES: .o .h .cxx
.cxx.o:
	$$($(FLTK_CONFIG) --cxx) -c $$($(FLTK_CONFIG) --cxxflags) $(CXXFLAGS) $<


SRCS=   GraphView.cxx DataModel.cxx Event.cxx InfoBar.cxx LocCommWin.cxx \
        ConcurrencyWin.cxx Settings.cxx  Fl_MultiGroup.cxx \
        ZoomScroll.cxx GridView.cxx MenuManager.cxx LocCommBox.cxx \
	ConcurrencyView.cxx  ProfileBrowser.cxx SelectBrowser.cxx SubView.cxx
HFILES= GraphView.h   DataModel.h   Event.h   InfoBar.h   LocCommWin.h \
        ConcurrencyWin.h   Settings.h Fl_MultiGroup.H \
        ZoomScroll.h GridView.h MenuManager.h LocCommBox.h \
	ConcurrencyView.h ProfileBrowser.h SelectBrowser.cxx Subview.h

GENSRCS= chplvis.cxx
GENH= chplvis.h

OFILES= $(SRCS:.cxx=.o) $(GENSRCS:.cxx=.o)

all: chplvis

chplvis: $(GENSRCS) $(OFILES) 
	$$($(FLTK_CONFIG) --cxx)  -o chplvis $(OFILES) \
	        $$($(FLTK_CONFIG) --ldflags) $$($(FLTK_CONFIG) --libs)

chplvis.h: chplvis.fl
	$(FLTK_FLUID) -c chplvis.fl

testMG.cxx: testMG.fl
	$(FLTK_FLUID) -c testMG.fl

testMG: Fl_MultiGroup.o testMG.o
	$$($(FLTK_CONFIG) --cxx)  -o testMG  Fl_MultiGroup.o testMG.o \
	       $$($(FLTK_CONFIG) --ldflags) $$($(FLTK_CONFIG) --libs)

clean:
	rm -f $(GENSRCS) $(GENH) $(OFILES) *~
	rm -f testMG.o testMG.cxx testMG.h testMG

clobber: clean
	rm -f chplvis

install: chplvis
	cp chplvis $(CHPL_BIN_DIR)

# Dependencies
chplvis.cxx: chplvis.h  # This rule to get fluid run only once
chplvis.o: chplvis.h Fl_MultiGroup.H Settings.h DataModel.h InfoBar.h \
     MenuManager.h ZoomScroll.h GraphView.h GridView.h LocCommWin.h  \
     ConcurrencyWin.h ConcurrencyView.h
GraphView.o:  chplvis.h Fl_MultiGroup.H Settings.h DataModel.h InfoBar.h \
     MenuManager.h ZoomScroll.h GraphView.h GridView.h LocCommWin.h  \
     ConcurrencyWin.h ConcurrencyView.h
GridView.o:  chplvis.h Fl_MultiGroup.H Settings.h DataModel.h InfoBar.h \
     MenuManager.h ZoomScroll.h GraphView.h GridView.h LocCommWin.h  \
     ConcurrencyWin.h ConcurrencyView.h
InfoBar.o: chplvis.h Fl_MultiGroup.H Settings.h DataModel.h InfoBar.h \
     MenuManager.h ZoomScroll.h GraphView.h GridView.h LocCommWin.h  \
     ConcurrencyWin.h LocCommBox.h ConcurrencyView.h
Settings.o: chplvis.h Fl_MultiGroup.H Settings.h DataModel.h InfoBar.h \
     MenuManager.h ZoomScroll.h GraphView.h GridView.h LocCommWin.h  \
     ConcurrencyWin.h ConcurrencyView.h
ZoomScroll.o:   chplvis.h Fl_MultiGroup.H Settings.h DataModel.h InfoBar.h \
     MenuManager.h ZoomScroll.h GraphView.h GridView.h LocCommWin.h  \
     ConcurrencyWin.h ConcurrencyView.h
MenuManager.o:  chplvis.h Fl_MultiGroup.H Settings.h DataModel.h InfoBar.h \
     MenuManager.h ZoomScroll.h GraphView.h GridView.h LocCommWin.h  \
     ConcurrencyWin.h ConcurrencyView.h
ConcurrencyWin.o: chplvis.h Fl_MultiGroup.H Settings.h DataModel.h InfoBar.h \
     MenuManager.h ZoomScroll.h GraphView.h GridView.h LocCommWin.h  \
     ConcurrencyWin.h ConcurrencyView.h
ProfileBrowser.o: chplvis.h Fl_MultiGroup.H Settings.h DataModel.h InfoBar.h \
     MenuManager.h ZoomScroll.h GraphView.h GridView.h LocCommWin.h  \
     ConcurrencyWin.h ConcurrencyView.h SelectBrowser.h SubView.h
SubView.o:  chplvis.h Fl_MultiGroup.H Settings.h DataModel.h InfoBar.h \
     MenuManager.h ZoomScroll.h GraphView.h GridView.h LocCommWin.h  \
     ConcurrencyWin.h ConcurrencyView.h SelectBrowser.h SubView.h
Event.o: Event.h
DataModel.o: DataModel.h StringCache.h
Fl_MultiGroup.o: Fl_MultiGroup.H 
LocCommWin.o: LocCommWin.h GraphView.h  ConcurrencyWin.h DataModel.h \
    DataView.h ConcurrencyView.h
LocCommBox.o: LocCommBox.h
ConcurrencyView.o: chplvis.h  Fl_MultiGroup.H Settings.h DataModel.h InfoBar.h \
     MenuManager.h ZoomScroll.h GraphView.h GridView.h LocCommWin.h
SelectBrowser.o: SelectBrowser.h
