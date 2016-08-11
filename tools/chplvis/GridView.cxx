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

// This class is the primary view field for chplvis.   Most of the visualization
// is done by special drawing done by this class


#include "chplvis.h"
#include "math.h"
#include <FL/fl_draw.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Menu_Item.H>


//  GridView Constructors

GridView::GridView (int bx, int by, int bw, int bh, const char *label)
  : DataView (bx, by, bw, bh, 0) 
{
  // printf ("GridView init. h=%d, w=%d, numlocales is %d\n",bh,bw, VisData.NumLocales());
  numlocales = 1;
  theLocales = NULL;
  comms = NULL;
  curTagData = NULL;
  if (VisData.NumLocales() > 0) {
    setNumLocales(VisData.NumLocales());
  }
  boxSize = 10;
};

// Private methods 

void GridView::allocArrays()
{
  int ix;
  int ix2;

  //printf ("allocArrays\n");
  // Dealloc anything current
  if (theLocales != NULL) {
    for (ix = 0; ix < getSize; ix++) {
      if (theLocales[ix].win != NULL) {
        delete theLocales[ix].win;
        //printf ("deleting win for %d\n", ix);
      }
      if (theLocales[ix].ccwin != NULL) 
        delete theLocales[ix].ccwin;
      if (theLocales[ix].b != NULL) 
        delete theLocales[ix].b;
    }
    delete [] theLocales;
  }
  if (comms != NULL) {
    for (ix = 0; ix < getSize; ix++)  { 
      for (ix2 = 0; ix2 < getSize; ix2++) 
        if (comms[ix][ix2].win != NULL)
          delete comms[ix][ix2].win;
      delete [] comms[ix];
    }
    delete [] comms;
  }
  
  // Alloc new space
  theLocales = new localeInfo [numlocales];
  comms = new  struct commInfo* [numlocales];
  if (theLocales == NULL || comms == NULL) {
    fprintf (stderr, "chplvis: out of memory.\n");
    exit(1);
  }
  getSize = numlocales;
  for (ix = 0; ix < getSize; ix++) {
    comms[ix] = new struct commInfo[numlocales];
    if (comms[ix] == NULL) {
      fprintf (stderr, "chplvis: out of memory.\n");
      exit(1);
    }
    for (ix2 = 0; ix2 < numlocales; ix2++) {
      comms[ix][ix2].win = NULL;
    }
  }

  for (ix = 0; ix < numlocales; ix++) {
    theLocales[ix].win = NULL;
    theLocales[ix].ccwin = NULL;
    theLocales[ix].b = NULL;
  }
}


// Public Methods 

//  tagNo:  -3 => new open
//          -2 => All: start to finish
//          -1 => Start to first tag (tag 0)
//          0 and greater => start at tagNo to next tag.

void GridView::selectData(int tagNum)
{
  int ix1, ix2;  // For processing the arrays

  // if (tagNum < TagALL || tagNum >= numTags) error of some kind

  // Close all the windows
  for (ix1 = 0; ix1 < numlocales; ix1++) {
    if (theLocales[ix1].win != NULL) {
      theLocales[ix1].win->hide();
    }
    if (theLocales[ix1].ccwin != NULL) {
      theLocales[ix1].ccwin->hide();
    }
    for (ix2 = 0; ix2 < numlocales; ix2++)  {
      if (comms[ix1][ix2].win != NULL) 
        comms[ix1][ix2].win->hide();
    }
  }

  // Select data to use
  if (Menus.usingUTags())
    curTagData = VisData.getUTagData(tagNum);
  else
    curTagData = VisData.getTagData(tagNum);
  curTagNum = tagNum;

  // Set the max values in the info bar
  Info->setMaxes(curTagData->maxTasks, curTagData->maxComms, curTagData->maxSize,
                 curTagData->maxConc, curTagData->maxCpu, curTagData->maxClock);

 }

void GridView::setTooltip ( int ix, bool isInt, int ival, double fval)
{
#if 0
    char tmpchars[100];
    if (theLocales != NULL) {
      localeInfo *loc = &theLocales[ix];

      // Sets up an invisible box under each locale that responds to tooltip requests!
      loc->x = cx + (int) rint(rx * sin (angle * ix - start));
      loc->y = cy - (int) rint(ry * cos (angle * ix - start));
      loc->w = 30;
      loc->h = 30;

      // Invisible button for the tooltip!
      if (loc->b == NULL) {
        loc->b = new Fl_Box (FL_NO_BOX, loc->x-loc->w/2, loc->y-loc->h/2, loc->w, loc->h, NULL);
        parent()->add(loc->b);
        loc->b->show();
      } else {
        loc->b->position(loc->x-loc->w/2,loc->y-loc->h/2);
        loc->b->size(loc->w,loc->h);
      }
      if (isInt)
        snprintf (tmpchars, sizeof(tmpchars), "%d", ival);
      else
        snprintf (tmpchars, sizeof(tmpchars), "%lf", fval);
      loc->b->copy_tooltip(tmpchars);
    }
#endif
}


void GridView::drawLocale ( int ix, Fl_Color col)
{
  //printf ("drawLocale %d\n", ix);
  char ixchars[10];
  if (theLocales != NULL) {
    localeInfo *loc = &theLocales[ix];

    // Draw a locale ...
    loc->x = 30 + boxSize*ix;
    loc->y = 30;
    loc->w = boxSize;
    loc->h = boxSize;

    fl_color(col);
    fl_rectf(x() + 10 + boxSize + loc->x, y() + loc->y, loc->w, loc->h);
    fl_rectf(x() + loc->y, y() + 10 + boxSize + loc->x, loc->w, loc->h);
    fl_color(FL_BLACK);
    fl_rect(x() + 10 + boxSize + loc->x, y() + loc->y, loc->w+1, loc->h);
    fl_rect(x() + loc->y, y() + 10 + boxSize + loc->x, loc->w, loc->h+1);

    if (boxSize < 30) {
      if (!(ix % (numlocales/4)) || ix == numlocales-1) {
        snprintf (ixchars, 10, "%d", ix);
        fl_draw(ixchars, x() + 10 + boxSize + loc->x, y() + 5,
                boxSize, 20, FL_ALIGN_CENTER, NULL, 0);
        fl_draw(ixchars, x() + 5, y() + (int)(1.5 * boxSize) + loc->x,
                20, 20, FL_ALIGN_CENTER, NULL, 0);
      }
    } else {
      snprintf (ixchars, 10, "%d", ix);
      fl_draw(ixchars, x() + 10 + boxSize + loc->x, y() + loc->y,
              loc->w, loc->h, FL_ALIGN_CENTER, NULL, 0);
      fl_draw(ixchars, x() + loc->y, y() + 10 + boxSize + loc->x,
              loc->w, loc->h, FL_ALIGN_CENTER, NULL, 0);
    }
  }
}

void GridView::drawCommBox (int ix1, Fl_Color col1,  int ix2, Fl_Color col2)
{
  int x1, x2, y1, y2;

  if (ix1 < 0 || ix1 >= numlocales || ix2 < 0 || ix2 >= numlocales) {
    fprintf (stderr, "DATA ERROR: drawCommBox\n");
    return;
  }
  localeInfo *loc1 = &theLocales[ix1], *loc2 = &theLocales[ix2];

  x1 = x() + 10 + boxSize + loc1->x;
  y1 = y() + 10 + boxSize + loc2->x;

  x2 = x() + 10 + boxSize + loc2->x;
  y2 = y() + 10 + boxSize + loc1->x;

  fl_color(col1);
  fl_rectf(x1, y1, loc1->w, loc1->h);
  fl_color(FL_BLACK);
  fl_rect(x1, y1, loc1->w+1, loc1->h+1);

  fl_color(col2);
  fl_rectf(x2, y2, loc1->w, loc1->h);
  fl_color(FL_BLACK);
  fl_rect(x2, y2, loc1->w+1, loc1->h+1);

  // printf ("1: (%d, %d->%d),  2: (%d, %d->%d)\n", x1, y1, (int)col1, x2, y2, (int) col2);
}

void GridView::draw()
{
  int ix;
  int iy;

  //printf ("GridView draw, numlocales is %d\n", numlocales);
  //printf ("draw: x,y = %d,%d, w,h = %d,%d\n", x(), y(), w(), h());
  //printf ("zoom: x,y = %d,%d, w,h = %d,%d\n", GridScroll->x(), GridScroll->y(), GridScroll->w(), GridScroll->h());

#if 0
  for (ix = 0; ix < numlocales; ix++) {
    switch (Info->dataToShow()) {
    case show_Tasks:
      setTooltip(ix, true, curTagData->locales[ix].numTasks, 0);
      break;
    case show_CPU:
      setTooltip(ix, false, 0, curTagData->locales[ix].Cpu);
      break;
    case show_Clock:
      setTooltip(ix, false, 0, curTagData->locales[ix].clockTime);
      break;
    case show_Concurrency:
      setTooltip(ix, true, curTagData->locales[ix].maxConc, 0);
    }
  }
#endif

  DataView::draw();

  // Draw locales first
  
  for (ix = 0; ix < numlocales; ix++) {
    switch (Info->dataToShow()) {
    case show_Tasks:
      drawLocale(ix, heatColor(curTagData->locales[ix].numTasks, curTagData->maxTasks));
      break;
    case show_CPU:
      drawLocale(ix, heatColor(curTagData->locales[ix].Cpu, curTagData->maxCpu));
      break;
    case show_Clock:
      drawLocale(ix, heatColor(curTagData->locales[ix].clockTime, curTagData->maxClock));
      break;
    case show_Concurrency:
      drawLocale(ix, heatColor(curTagData->locales[ix].maxConc, curTagData->maxConc));
    }
  }

  for (ix = 0; ix < numlocales-1; ix++) {
    for (iy = ix + 1; iy < numlocales; iy++) {
      int  com2ix, com2iy, comMax;
      if (Info->commToShow()) {
        com2ix = curTagData->comms[iy][ix].numComms;
        com2iy = curTagData->comms[ix][iy].numComms;
        comMax = curTagData->maxComms;
      } else {
        com2ix = curTagData->comms[iy][ix].commSize;
        com2iy = curTagData->comms[ix][iy].commSize;
        comMax = curTagData->maxSize;
      }
      // Draw white if no communications
      drawCommBox(ix, com2ix ? heatColor(com2ix, comMax) : FL_WHITE,
                   iy, com2iy ? heatColor(com2iy, comMax) : FL_WHITE);
    }
  }
    
  Info->draw();
}

int GridView::handle(int event)
{
  int ix;

  int x = Fl::event_x();
  int y = Fl::event_y();

  switch (event) {
  case FL_PUSH:
    //printf ("Push at (%d,%d) event_button() = %d\n", x, y, Fl::event_buttons());
    //if (Fl::event_buttons() == FL_RIGHT_MOUSE) {
    //  printf ("right mouse push\n");
    //}
    break;
  case FL_RELEASE:
    //printf ("Release at (%d,%d)\n", x, y);
    if (Fl::event_button() == FL_MIDDLE_MOUSE ||
        Fl::event_button() == FL_RIGHT_MOUSE) {
      // printf ("middle or right release\n");
      break;
    }
    // Click on a locale?
    if (numlocales > 0) {
      for (ix = 0; ix < numlocales; ix++) {
        // See if release is inside a locale
        localeInfo *loc = &theLocales[ix];
        if ( x > loc->x-loc->w/2 && x <= loc->x + loc->w/2 &&
             y > loc->y-loc->h/2 && y <= loc->y + loc->h/2) {
          if (Info->dataToShow() == show_Concurrency) {
            if (Menus.usingUTags() && curTagNum != DataModel::TagALL) {
              fl_alert("Concurrency view available only for tag 'ALL' in merged tag mode.");
            } else {
              if (theLocales[ix].ccwin == NULL) {
                // Create the window
                theLocales[ix].ccwin = make_concurrency_window(ix, curTagNum);
              } else {
                theLocales[ix].ccwin->updateData(ix, curTagNum);
                theLocales[ix].ccwin->showTaskBox();
              }
              if (theLocales[ix].ccwin->visible())
                theLocales[ix].ccwin->hide();
              else
                theLocales[ix].ccwin->show();
            }
          } else {
            if (theLocales[ix].win == NULL) {
              // Create the window
              theLocales[ix].win = make_locale_window(ix, &curTagData->locales[ix]);
            } else {
              theLocales[ix].win->updateWin(&curTagData->locales[ix]);
            }
            if (theLocales[ix].win->visible()) 
              theLocales[ix].win->hide();
            else
              theLocales[ix].win->show();
          }
          return 1;
        }
      }
    }

#if 0
    // Click on a comm link?
    for (i = 0; i < numlocales; i++) {
      localeInfo *loci, *locj;
      loci = &theLocales[i];
      for (j = i+1; j < numlocales; j++) {
        locj = &theLocales[j];
        // printf ("link: %d->%d:\n", i, j);
        if (curTagData->comms[i][j].numComms != 0
            || curTagData->comms[j][i].numComms != 0) {
          int OnComm = isOnCommLink(x,y,loci,locj);
          if (OnComm) {
            // printf ("Link %d -> %d, nearer locale %d\n", i, j, (OnComm > 1 ? j : i ));
            if (OnComm == 1) {
              // j -> i, swap so both can use i->j
              int t = j; j = i; i = t;
            } 
            if (comms[i][j].win == NULL) {
              comms[i][j].win = make_comm_window(i,j,&curTagData->comms[i][j]);
            } else {
              comms[i][j].win->updateWin(&curTagData->comms[i][j]);
            }
            if (comms[i][j].win->visible())
              comms[i][j].win->hide();
            else
              comms[i][j].win->show();
            return 1;
          }
        }
      }
    }
#endif
    
    break;
  }
  return DataView::handle(event);
}

void GridView::redrawAllWindows(void)
    {
      int ix, ix1, ix2;
      MainWindow->redraw();
      for (ix = 0; ix < numlocales; ix++) {
        if (theLocales[ix].win != NULL)
          theLocales[ix].win->redraw();
        if (theLocales[ix].ccwin != NULL)
          theLocales[ix].ccwin->redraw();
      }
      for (ix1 = 0; ix1 < numlocales; ix1++)
        for (ix2 = 0; ix2 < numlocales; ix2++)
          if (comms[ix1][ix2].win != NULL)
            comms[ix1][ix2].win->redraw();
    }


void GridView::resize(int X, int Y, int W, int H)
{
  //printf ("GridView::resize(%d,%d,%d,%d)\n", X, Y, W, H);
  if (H < W) W = H;
  if (W < (numlocales+2)*minBoxSize+40) W = (numlocales+2)*minBoxSize+40;
  boxSize = ((W-40)/(numlocales+2));
  //printf ("Fl_Box::resize(%d,%d,%d,%d)\n", X, Y, W, H);
  Fl_Box::resize(X,Y,W,W);
}
