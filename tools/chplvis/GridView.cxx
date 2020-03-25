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
  getSize = 0;
};

// Private methods

void GridView::allocArrays()
{
  int ix;
  int ix2;

  // printf ("allocArrays: getSize = %d, numLocales = %d\n", getSize, numlocales);
  // Dealloc anything current
  if (theLocales != NULL) {
    for (ix = 0; ix < getSize; ix++) {
      // if (theLocales[ix].win != NULL) {
      //  delete theLocales[ix].win;
      //  printf ("deleting win for %d\n", ix);
      // }
      //if (theLocales[ix].ccwin != NULL)
      //  delete theLocales[ix].ccwin;
      //if (theLocales[ix].bT != NULL)
      //  delete theLocales[ix].bT;
      if (theLocales[ix].bL != NULL)
        delete theLocales[ix].bL;
    }
    delete [] theLocales;
  }
  if (comms != NULL) {
    for (ix = 0; ix < getSize; ix++)  {
      //for (ix2 = 0; ix2 < getSize; ix2++)
        //if (comms[ix][ix2].win != NULL)
        //  delete comms[ix][ix2].win;
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
      // comms[ix][ix2].win = NULL;
      comms[ix][ix2].b = NULL;
    }
  }

  for (ix = 0; ix < numlocales; ix++) {
    //theLocales[ix].win = NULL;
    //theLocales[ix].ccwin = NULL;
    theLocales[ix].bT = NULL;
    theLocales[ix].bL = NULL;
  }
}


// Public Methods

//  tagNo:  -3 => new open
//          -2 => All: start to finish
//          -1 => Start to first tag (tag 0)
//          0 and greater => start at tagNo to next tag.

void GridView::selectData(int tagNum)
{
  // int ix1, ix2;  // For processing the arrays

  // if (tagNum < TagALL || tagNum >= numTags) error of some kind

#if 0
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
#endif

  // Select data to use
  if (Menus.usingUTags())
    curTagData = VisData.getUTagData(tagNum);
  else
    curTagData = VisData.getTagData(tagNum);
  curTagNum = tagNum;

  // Set the max values in the info bar
  Info->setMaxes(curTagData->maxTasks, curTagData->maxComms, curTagData->maxSize,
                 curTagData->maxConc, curTagData->maxCpu, curTagData->maxClock);
  Info->rmAllLocOrComm();
 }

void GridView::setLocTooltip ( int ix, bool isInt, int ival, double fval)
{
  char tmpchars[100];
  if (theLocales != NULL) {
    localeInfo *loc = &theLocales[ix];

    // Invisible button for the tooltip!
    if (loc->bT == NULL) {
      loc->bT = new Fl_Box (FL_NO_BOX, x() + 10 + boxSize + loc->x,
                            y() + loc->y, loc->w, loc->h, NULL);
      parent()->add(loc->bT);
      loc->bT->show();
      loc->bL = new Fl_Box (FL_NO_BOX, x() + loc->y,
                            y() + 10 + boxSize + loc->x, loc->w, loc->h, NULL);
      parent()->add(loc->bL);
      loc->bL->show();
    } else {
      loc->bT->resize(x() + 10 + boxSize + loc->x,
                      y() + loc->y, loc->w,loc->h);
      loc->bL->resize( x() + loc->y,
                       y() + 10 + boxSize + loc->x, loc->w, loc->h);
    }
    if (isInt)
      snprintf (tmpchars, sizeof(tmpchars), "%d", ival);
    else
      snprintf (tmpchars, sizeof(tmpchars), "%lf", fval);
    loc->bT->copy_tooltip(tmpchars);
    loc->bL->copy_tooltip(tmpchars);
  }
}

void GridView::setCommTooltip (int i, int j, int val)
{
  char data[30];
  if (val) {
    if (!comms[i][j].b) {
      comms[i][j].b = new Fl_Box(FL_NO_BOX, comms[i][j].x, comms[i][j].y,
                                 boxSize, boxSize, NULL);
      parent()->add(comms[i][j].b);
      comms[i][j].b->show();
    }
    snprintf (data, 30, "%d", val);
    comms[i][j].b->copy_tooltip(data);
  } else {
    if (comms[i][j].b)
      comms[i][j].b->hide();
  }
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

  comms[ix1][ix2].x = x1 = x() + 10 + boxSize + loc1->x;
  comms[ix1][ix2].y = y1 = y() + 10 + boxSize + loc2->x;

  comms[ix2][ix1].x = x2 = x() + 10 + boxSize + loc2->x;
  comms[ix2][ix1].y = y2 = y() + 10 + boxSize + loc1->x;

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

  DataView::draw();

  // Draw locales first

  for (ix = 0; ix < numlocales; ix++) {
    switch (Info->dataToShow()) {
    case show_Tasks:
      drawLocale(ix, heatColor(curTagData->locales[ix].numTasks, curTagData->maxTasks));
      setLocTooltip(ix, true, curTagData->locales[ix].numTasks, 0);
      break;
    case show_CPU:
      drawLocale(ix, heatColor(curTagData->locales[ix].Cpu, curTagData->maxCpu));
      setLocTooltip(ix, false, 0, curTagData->locales[ix].Cpu);
      break;
    case show_Clock:
      drawLocale(ix, heatColor(curTagData->locales[ix].clockTime, curTagData->maxClock));
      setLocTooltip(ix, false, 0, curTagData->locales[ix].clockTime);
      break;
    case show_Concurrency:
      drawLocale(ix, heatColor(curTagData->locales[ix].maxConc, curTagData->maxConc));
      setLocTooltip(ix, true, curTagData->locales[ix].maxConc, 0);
    }
  }

  for (ix = 0; ix < numlocales; ix++) {
    for (iy = ix; iy < numlocales; iy++) {
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
      setCommTooltip(ix, iy, com2ix);
      setCommTooltip(iy, ix, com2iy);
    }
  }

  Info->draw();
}

int GridView::handle(int event)
{
  int x = Fl::event_x() - Grid_View->x() - 30;
  int y = Fl::event_y() - Grid_View->y() - 30;

  int loc1, loc2;

  switch (event) {
  case FL_PUSH:
    //printf ("Push at (%d,%d) event_button() = %d\n", x, y, Fl::event_buttons());
    //if (Fl::event_buttons() == FL_RIGHT_MOUSE) {
    //  printf ("right mouse push\n");
    //}
    break;
  case FL_RELEASE:
    //printf ("Release at (%d,%d), boxsize is %d\n", x, y, boxSize);
    if (Fl::event_button() == FL_RIGHT_MOUSE) {
      // printf ("right release\n");
      break;
    }
    // Calculate which one.
    if (x <= boxSize && y > 12+boxSize) {
      loc1 = (y-12-boxSize)/boxSize;
      if (loc1 >= 0 && loc1 < numlocales) {
        //printf ("left row of locales, %d.\n", loc1);
        Menus.setCurrentLoc(loc1);
        if (Info->dataToShow() == show_Concurrency) {
          if (Menus.usingUTags() && curTagNum != DataModel::TagALL) {
            fl_alert("Concurrency view available only for tag 'ALL' in merged tag mode.");
          } else {
            if (Fl::event_button() == FL_MIDDLE_MOUSE) {
#if 0
              if (theLocales[loc1].ccwin == NULL) {
                // Create the window
                theLocales[loc1].ccwin = make_concurrency_window(loc1, curTagNum);
              } else {
                theLocales[loc1].ccwin->updateData(loc1, curTagNum);
                theLocales[loc1].ccwin->showTaskBox();
              }
              if (theLocales[loc1].ccwin->visible())
                theLocales[loc1].ccwin->hide();
              else
                theLocales[loc1].ccwin->show();
#endif
              return 1; // disable extra window mode
            } else {
              // Show the concurrency view.
              DataField->selectChild(concView);
              concView->updateData(loc1, curTagNum);
            }
          }
        } else {
          if (Fl::event_button() == FL_MIDDLE_MOUSE){
#if 0
            //printf ("Making locale window.\n");
            if (theLocales[loc1].win == NULL) {
              // Create the window
              theLocales[loc1].win = make_LC_window(loc1, &curTagData->locales[loc1]);
            } else {
              theLocales[loc1].win->setAsLocale(loc1, &curTagData->locales[loc1]);
            }
            if (theLocales[loc1].win->visible())
              theLocales[loc1].win->hide();
            else
              theLocales[loc1].win->show();
#endif
            return 0; // disable window mode
          } else {
            // Left mouse, place it on the info bar.
            LocCommBox *infoBox;
            infoBox = Info->getNewLocComm();
            infoBox->setLocale(loc1, &curTagData->locales[loc1]);
            infoBox->addXButton();
            Info->addLocOrComm(infoBox);
          }
          return 1;
        }
      }
    } else if (y <= boxSize && x > 12+boxSize ) {
      loc1 = (x-12-boxSize)/boxSize;
      if (loc1 >= 0 && loc1 < numlocales) {
        // printf ("top row of locales, %d.\n", loc1);
        Menus.setCurrentLoc(loc1);
        if (Info->dataToShow() == show_Concurrency) {
          if (Menus.usingUTags() && curTagNum != DataModel::TagALL) {
            fl_alert("Concurrency view available only for tag 'ALL' in merged tag mode.");
          } else {
            if (Fl::event_button() == FL_MIDDLE_MOUSE) {
#if 0
              if (theLocales[loc1].ccwin == NULL) {
                // Create the window
                theLocales[loc1].ccwin = make_concurrency_window(loc1, curTagNum);
              } else {
                theLocales[loc1].ccwin->updateData(loc1, curTagNum);
                theLocales[loc1].ccwin->showTaskBox();
              }
              if (theLocales[loc1].ccwin->visible())
                theLocales[loc1].ccwin->hide();
              else
                theLocales[loc1].ccwin->show();
#endif
              return 1;  // disable extra window mode
            } else {
              // Show the concurrency view.
              DataField->selectChild(concView);
              concView->updateData(loc1, curTagNum);
            }
          }
        } else {
          if (Fl::event_button() == FL_MIDDLE_MOUSE){
#if 0
            printf ("Making locale window.\n");
            if (theLocales[loc1].win == NULL) {
              // Create the window
              theLocales[loc1].win = make_LC_window(loc1, &curTagData->locales[loc1]);
            } else {
              theLocales[loc1].win->setAsLocale(loc1, &curTagData->locales[loc1]);
            }
            if (theLocales[loc1].win->visible())
              theLocales[loc1].win->hide();
            else
              theLocales[loc1].win->show();
#endif
            return 0; // Disable LC windows
          } else {
            // Left mouse, place it on the info bar.
            LocCommBox *infoBox;
            infoBox = Info->getNewLocComm();
            infoBox->setLocale(loc1, &curTagData->locales[loc1]);
            infoBox->addXButton();
            Info->addLocOrComm(infoBox);
          }
          return 1;
        }
      }
    } else {
      y -= boxSize + 10;
      x -= boxSize + 10;
      if (x > 0 && y > 0) {
        loc1 = y/boxSize;
        loc2 = x/boxSize;
        if (loc1 < numlocales && loc2 < numlocales && loc1 != loc2 ) {
          // printf ("Comm, %d,%d\n", loc1, loc2);
          if (Fl::event_button() == FL_MIDDLE_MOUSE) {
#if 0
            //printf ("Should create a comm win.\n");
            if (comms[loc1][loc2].win == NULL) {
              comms[loc1][loc2].win = make_LC_window(loc1, loc2,
                                          &curTagData->comms[loc1][loc2]);
            } else {
              comms[loc1][loc2].win->setAsComm(loc1, loc2,
                                          &curTagData->comms[loc1][loc2]);
            }
            if (comms[loc1][loc2].win->visible())
              comms[loc1][loc2].win->hide();
            else
              comms[loc1][loc2].win->show();
#endif
            return DataView::handle(event);  // disabled LC windows
          } else {
            // printf ("New LocCommBox for comm...\n");
            if (curTagData->comms[loc1][loc2].numComms == 0)
              return DataView::handle(event);
            LocCommBox *infoBox;
            infoBox = Info->getNewLocComm();
            infoBox->setComm(loc1, loc2, &curTagData->comms[loc1][loc2]);
            infoBox->addXButton();
            Info->addLocOrComm(infoBox);
          }
          return 1;
        }
      }
    }
    break;
  }
  return DataView::handle(event);
}

void GridView::redrawAllWindows(void)
    {
//    int ix, ix1, ix2;
      MainWindow->redraw();
#if 0
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
#endif
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
