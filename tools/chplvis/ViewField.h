/*
 * Copyright 2015 Cray Inc.
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

#ifndef VIEWFIELD_H
#define VIEWFIELD_H

//class ViewField;  // So we can include chplvis.h

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>

#include <string>

#include "DataModel.h"

// extern Fl_Double_Window *MainWin;

struct localeInfo {
  int x;
  int y;
  int w;
  int h;
  int numTasks;
  double userCpu;
  double sysCpu;
  double Cpu;
  double refUserCpu; 
  double refSysCpu;
  Fl_Double_Window *win;
  // Fl_Color heat;
};

struct commInfo {
  int numGets;
  long commSize;
};

struct tagInfo {
  int tagNo;
  std::string tagName;
  
};

Fl_Color heatColor ( double val, double max );

static const double twopi = 6.28318530717958647688;
static const double pi = 3.14159265358979323844;

class ViewField : public Fl_Box {

  private:

    int numlocales;
    int cx, cy; // center of the ViewField
    double rx, ry; // Radius of the locales
    double angle;
    double start;

    localeInfo *theLocales;
    int getSize; // need this to deallocate after changing numlocales
    commInfo **comms;

    tagInfo *tags;

    bool showtasks;
    int maxTasks;
    double maxCpu;

    bool showcomms;
    int maxComms;
    long maxDatasize; 

    // Methods

    void allocArrays();

  public:

  ViewField (int bx, int by, int bw, int bh, const char *label = 0);

  ViewField (Fl_Boxtype b, int bx, int by, int bw, int bh,
             const char *label = 0);

  //  Virtual methods to override
  void draw(void);
  int handle(int event);

  // Processing routines

  void processData();

  void setNumLocales(int n)
    { 
      //printf("NumLocalse set to %d\n", n);
      numlocales = n;
      angle = twopi / numlocales;
      start = ( numlocales % 2 == 0 ? angle / 2 : 0 );
      allocArrays();
    }

  int  getNumLocales(void) { return numlocales; }

  // Draw a "locale box, with ix as the label on it
  void drawLocale(int ix, Fl_Color col);

  // Draw a comm line between loc1 and loc2, color changing in the middle
  void drawCommLine(int ix1, Fl_Color col1,  int ix2, Fl_Color col2);

  // What to show!
  void showTasks(void) { showtasks = true; }
  void showCpu(void) { showtasks = false; }
  void showComms(void) { showcomms = true; }
  void showDsize(void) { showcomms = false; }

};

#endif
