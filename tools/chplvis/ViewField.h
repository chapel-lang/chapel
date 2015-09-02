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
#include "LocaleWin.h"
#include "CommWin.h"
#include "ConcurrencyWin.h"

#include <string>

#include "DataModel.h"

// Information stored for each locale

struct localeInfo {
  // locale box location on view area
  int x; 
  int y;
  int w;
  int h;
  // Locale Window information.
  LocaleWin *win;
  // Concurrency Window information.
  ConcurrencyWin *ccwin;
};

// Information stored for every comm direction
// X -> Y and Y -> X for all X & Y.  (2d array)

struct commInfo { // Remove this and just use CommWin*??? YYY
  CommWin *win;
};

// Tag names may appear multiple times in the data,
// Associates unique tag number with name.

struct tagInfo {
  int tagNo;
  char *tagName;
};

// Utility routine for computing colors for display,
// Needed by other classes that draw colors.

Fl_Color heatColor ( double val, double max );

static const double twopi = 6.28318530717958647688;
static const double pi = 3.14159265358979323844;

class ViewField : public Fl_Box {

  private:

    int numlocales;
    int cx, cy;     // center of the ViewField
    double rx, ry;  // Radius of the locales, for elliptical view
    double angle;   // Angle between locale
    double start;   // Angle of locale 0

    // Data arrays for the locales (1D) and communication (2D)
    localeInfo *theLocales; // Need to de/reallocate after changing numlocales
    int getSize;            // size used for doing deallocate after changeing numlocales
    commInfo **comms;       // Also need to de/reallocate after changing numlocales

    tagInfo *tags;    // Information for creating the tag menu
    int tagsSize;
    int tagMenu;

    // Keep track of what is being displayed
    enum show_what {show_Tasks, show_CPU, show_Clock, show_Concurrency} infoTop;

    DataModel::tagData *curTagData;
    int curTagNum;
    bool showcomms;

    // Methods

    void allocArrays ();

  public:

  ViewField (int bx, int by, int bw, int bh, const char *label = 0);

  ViewField (Fl_Boxtype b, int bx, int by, int bw, int bh,
             const char *label = 0);

  //  Virtual methods to override
  void draw (void);
  int handle (int event);

  // Processing routines

  void selectData (int tagNum);

  void makeTagsMenu (void);

  void setNumLocales (int n)
    { 
      //printf("NumLocalse set to %d\n", n);
      numlocales = n;
      angle = twopi / numlocales;
      start = ( numlocales % 2 == 0 ? angle / 2 : 0 );
      allocArrays();
    }

  int  getNumLocales (void) { return numlocales; }

  // Draw a "locale box, with ix as the label on it
  void drawLocale (int ix, Fl_Color col);

  // Draw a comm line between loc1 and loc2, color changing in the middle
  void drawCommLine (int ix1, Fl_Color col1,  int ix2, Fl_Color col2);

  // What to show!
  void showTasks (void) { infoTop = show_Tasks; }
  void showCpu (void  ) { infoTop = show_CPU; }
  void showClock (void) { infoTop = show_Clock; }
  void showConcurrency (void) { infoTop = show_Concurrency; }

  void showComms (void) { showcomms = true; }
  void showDsize (void) { showcomms = false; }

  void showAllData (void) { printf ("showAllData called\n"); }

  // Window show/hide functions ...
  void hideAllCommWindows (void)
    {
      int ix1, ix2;
      for (ix1 = 0; ix1 < numlocales; ix1++)
        for (ix2 = 0; ix2 < numlocales; ix2++)
          if (comms[ix1][ix2].win != NULL)
            comms[ix1][ix2].win->hide();
    }

  void showAllCommWindows (void)
    {
      int ix1, ix2;
      for (ix1 = 0; ix1 < numlocales; ix1++)
        for (ix2 = 0; ix2 < numlocales; ix2++)
          if (comms[ix1][ix2].win != NULL)
            comms[ix1][ix2].win->show();
    }

  void hideAllLocaleWindows (void)
    {
      int ix;
      for (ix = 0; ix < numlocales; ix++) {
        if (theLocales[ix].win != NULL)
          theLocales[ix].win->hide();
        if (theLocales[ix].ccwin != NULL)
          theLocales[ix].ccwin->hide();
      }
    }        

  void showAllLocaleWindows (void)
    {
      int ix;
      for (ix = 0; ix < numlocales; ix++) {
        if (theLocales[ix].win != NULL)
          theLocales[ix].win->show();
        if (theLocales[ix].ccwin != NULL)
          theLocales[ix].ccwin->show();
      }
    }

};

#endif
