/*
 * Copyright 2016 Cray Inc.
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

#ifndef GRIDVIEW_H
#define GRIDVIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Menu_Button.H>

#include "DataModel.h"

#include <string>
#include <math.h>

class GridView : public Fl_Box {

  // Information stored for each locale

  struct localeInfo {
    // locale box location on view area
    int x; 
    int y;
    int w;
    int h;
    int size; // size of the box. Min 4?
    // Locale box ... for tool tips.
    Fl_Box *b;
  };
  
  // Tag names may appear multiple times in the data,
  // Associates unique tag number with name.
  
  struct tagInfo {
    int tagNo;
    char *tagName;
  };

  private:

    int numlocales;
    int nrows, ncols;  // Number of rows and columns

    // Data arrays for the locales
    localeInfo *theLocales; // Need to de/reallocate after changing numlocales
    int getSize;            // size used for doing deallocate after changeing numlocales

    bool useUTags;

    // Keep track of what is being displayed
    enum show_what {show_Tasks, show_CPU, show_Clock, show_Concurrency} infoTop;

    DataModel::tagData *curTagData;
    int curTagNum;
    bool showcomms;

    int tagMenu;
    int tagPopup;
    Fl_Menu_Button *popup;
    

    // Methods

    void allocArrays ();

  public:

  GridView (int bx, int by, int bw, int bh, const char *label = 0);

  //  Virtual methods to override
  void draw (void);
  int handle (int event);

  // Processing routines

  bool usingUTags() { return useUTags; }

  void toggleUTags() { useUTags = !useUTags; }

  void selectData (int tagNum);

  void makeTagsMenu (void);

  void setNumLocales (int n)
    { 
      printf("NumLocales set to %d\n", n);
      nrows = (int) sqrt((double)n);
      if (nrows*nrows < n) nrows++;
      ncols = nrows;
      numlocales = n;
      allocArrays();
    }

  int  getNumLocales (void) { return numlocales; }

  // Add an invisible under the locale
  void setTooltip ( int ix, bool isInt, int ival, double fval);

  // Draw a "locale box, with ix as the label on it
  void drawLocale (int ix, Fl_Color col);

  // What to show!
  void showTasks (void) { infoTop = show_Tasks; }
  void showCpu (void  ) { infoTop = show_CPU; }
  void showClock (void) { infoTop = show_Clock; }
  void showConcurrency (void) { infoTop = show_Concurrency; }

  void showComms (void) { showcomms = true; }
  void showDsize (void) { showcomms = false; }

  void redrawAllWindows(void);

};

#endif
