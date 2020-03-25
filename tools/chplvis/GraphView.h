/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2016-2019 Cray Inc.
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

#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

//class GraphView;  // So we can include chplvis.h

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include "LocCommWin.h"
#include "ConcurrencyWin.h"

#include "DataModel.h"
#include "DataView.h"

#include <string>

// Information stored for each locale

struct localeInfo {
  // locale box location on view area
  int x;
  int y;
  int w;
  int h;
  // Locale Window information.  -- code commented out
  // LocCommWin *win;
  // Concurrency Window information.
  // ConcurrencyWin *ccwin;
  // Locale box ... for tool tips.
  Fl_Box *b;
};

// Information stored for every comm direction
// X -> Y and Y -> X for all X & Y.  (2d array)

//struct commInfo { // Disabled, no multi-window mode.
//  LocCommWin *win;
//};

// Tag names may appear multiple times in the data,
// Associates unique tag number with name.

struct tagInfo {
  int tagNo;
  char *tagName;
};

static const double twopi = 6.28318530717958647688;
static const double pi = 3.14159265358979323844;

class GraphView : public DataView {

  private:

    int cx, cy;     // center of the GraphView
    double rx, ry;  // Radius of the locales, for elliptical view
    double angle;   // Angle between locale
    double start;   // Angle of locale 0

    // Data arrays for the locales (1D) and communication (2D)
    localeInfo *theLocales; // Need to de/reallocate after changing numlocales
    int getSize;            // size used for doing deallocate after changeing numlocales
    // commInfo **comms;       // Also need to de/reallocate after changing numlocales
    // Methods

    void allocArrays ();

  public:

  GraphView (int bx, int by, int bw, int bh, const char *label = 0);

  //  Virtual methods to override
  void draw (void);
  int handle (int event);

  // Processing routines

  void selectData (int tagNum);

  void setNumLocales (int n)
    {
      //printf("NumLocalse set to %d\n", n);
      numlocales = n;
      angle = twopi / numlocales;
      start = ( numlocales % 2 == 0 ? angle / 2 : 0 );
      allocArrays();
    }

  int  getNumLocales (void) { return numlocales; }

  // Add an invisible under the locale
  void setTooltip ( int ix, bool isInt, int ival, double fval);

  // Draw a "locale box, with ix as the label on it
  void drawLocale (int ix, Fl_Color col);

  // Draw a comm line between loc1 and loc2, color changing in the middle
  void drawCommLine (int ix1, Fl_Color col1,  int ix2, Fl_Color col2);

#if 0
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

  void hideAllLocCommWindows (void)
    {
      int ix;
      for (ix = 0; ix < numlocales; ix++) {
        if (theLocales[ix].win != NULL)
          theLocales[ix].win->hide();
        if (theLocales[ix].ccwin != NULL)
          theLocales[ix].ccwin->hide();
      }
    }

  void showAllLocCommWindows (void)
    {
      int ix;
      for (ix = 0; ix < numlocales; ix++) {
        if (theLocales[ix].win != NULL)
          theLocales[ix].win->show();
        if (theLocales[ix].ccwin != NULL)
          theLocales[ix].ccwin->show();
      }
    }
#endif

  void redrawAllWindows (void);

};

#endif
