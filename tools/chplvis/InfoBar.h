/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2015-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef INFOBAR_H
#define INFOBAR_H

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <string.h>
#include <list>
#include "LocCommBox.h"

// Top bar of display showing Max values and reference colors.

// Utility routine for computing colors for display,
// Needed by other classes that draw colors.

Fl_Color heatColor(double val, double max);

enum showWhat { show_Tasks, show_CPU, show_Clock, show_Concurrency };

class InfoBar : public Fl_Group {

 private:
  int maxTasks;
  int maxComms;
  long maxSize;
  double maxCpu;
  double maxClock;
  long maxConcurrent;
  showWhat infoTop;
  bool showcomms;
  bool showcolorref;
  bool showtag;

  char *fileName;
  char *tagName;

  std::list<LocCommBox *> infoBoxes;
  std::list<LocCommBox *> boxCache;

 public:

  InfoBar (int x, int y, int w, int h/*, const char *label = 0*/)
#ifdef __APPLE__
    : Fl_Group(x,0,w,h+20) {
#else
    : Fl_Group(x,y,w,h) {
#endif
    maxTasks = 0;
    maxComms = 0;
    maxSize = 0;
    maxCpu = 0;
    maxClock = 0;
    infoTop = show_Tasks;
    showcomms = true;
    showtag = true;
    showcolorref = true;
    fileName = NULL;
    tagName = NULL;
   };

  void draw(void);

  void setMaxes(int tasks, int comms, long size, long concurrent, double cpu,
                double clock) {
    maxTasks = tasks;
    maxComms = comms;
    maxSize = size;
    maxConcurrent = concurrent;
    maxCpu = cpu;
    maxClock = clock;
  }

  void setFileName(const char *name);
  void setTagName(const char *name);
  void clearTagName() { if (tagName != NULL) free(tagName); tagName = NULL; }
  char *getTagName() { return tagName; }

  void showTasks() { infoTop = show_Tasks; }
  void showCpu() { infoTop = show_CPU; }
  void showClock() { infoTop = show_Clock; }
  void showConcurrency() { infoTop = show_Concurrency; }
  void showComms() { showcomms = true; }
  void showSize() { showcomms = false; }

  void showColorRef() { showcolorref = true; }
  void hideColorRef() { showcolorref = false; }

  void showTag() { showtag = true; }
  void hideTag() { showtag = false; }

  showWhat dataToShow() { return infoTop; }
  bool commToShow() { return showcomms; }

  void addLocOrComm(LocCommBox *box);
  void delLocOrComm(LocCommBox *box);
  void rmAllLocOrComm();

  LocCommBox *getNewLocComm(void);

  bool isOnList (LocCommBox *box);

  void resize (int X, int Y, int W, int H);

};

#endif

