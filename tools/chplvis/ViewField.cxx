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

// This class is the primary view field for chplvis.   Most of the visualization
// is done by special drawing done by this class


#include "chplvis.h"
#include "ViewField.h"
#include "math.h"
#include <FL/fl_draw.H>

// Local utility functions

// Computation of color for use in displays

Fl_Color heatColor ( double val, double max ) {
  if (val == 0) return FL_WHITE;
  if (max == 1) return fl_rgb_color(255,0,0);
  if (val == 1) return fl_rgb_color(0,180,255);
  return fl_rgb_color( 255*(val/max), 180 * ((max-val)/max), 255 * ((max-val)/max));
}

//  ViewField Constructors


ViewField::ViewField (int bx, int by, int bw, int bh, const char *label)
  : Fl_Box (bx, by, bw, bh, 0) 
{
  // printf ("ViewField init. h=%d, w=%d, numlocales is %d\n",bh,bw, VisData.NumLocales());
  numlocales = 0;
  theLocales = NULL;
  comms = NULL;
  tags = NULL;
  curTagData = NULL;
  tagsSize = 0;
  tagMenu = -1;
  if (VisData.NumLocales() > 0) {
    setNumLocales(VisData.NumLocales());
  }
  /*
  maxTasks = 1;
  maxComms = 1;
  maxCpu = 0.000001;
  maxClock = 0;
  maxDatasize = 1;
  */
  infoTop = show_Tasks;
  showcomms = true;
  
};

ViewField::ViewField (Fl_Boxtype b, int bx, int by, int bw, int bh, const char *label)
  : Fl_Box (b, bx, by, bw, bh, 0)
{
  // printf ("ViewField init with boxtype. h=%d, w=%d\n",bh,bw);
  numlocales = 0;
  theLocales = NULL;
  comms = NULL;
  tags = NULL;
  curTagData = NULL;
  tagsSize = 0;
  tagMenu = -1;
  if (VisData.NumLocales() > 0) {
    setNumLocales(VisData.NumLocales());
  }
  /*
  maxTasks = 1;
  maxComms = 1;
  maxCpu = 0.000001;
  maxClock = 0;
  maxDatasize = 1;
  */
  infoTop = show_Tasks;
  showcomms = true;
};


// Private methods 

void ViewField::allocArrays()
{
  int ix;
  int ix2;

  //printf ("allocArrays\n");
  // Dealloc anything current
  if (theLocales != NULL) {
    for (ix = 0; ix < getSize; ix++)
      if (theLocales[ix].win != NULL) {
        delete theLocales[ix].win;
        //printf ("deleting win for %d\n", ix);
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
  if (tags != NULL) {
    delete [] tags;
    tags = NULL;
  }
  
  // Alloc new space ... except for tags, done in process data.
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
  }

  if (tags != NULL) {
    for (ix = 0; ix < tagsSize; ix++)
      if (tags[ix].tagName != NULL)
        delete [] tags[ix].tagName;
    delete [] tags;
  }
  tagsSize = VisData.NumTags();
  tags = new tagInfo [tagsSize];
  for (ix = 0; ix < VisData.NumTags(); ix++) {
    tags[ix].tagNo = -1;
    tags[ix].tagName = NULL;
  }
}


// Public Methods 

//  tagNo:  -3 => new open
//          -2 => All: start to finish
//          -1 => Start to first tag (tag 0)
//          0 and greater => start at tagNo to next tag.

void ViewField::selectData(int tagNum)
{
  int ix1, ix2;  // For processing the arrays

  // if (tagNum < TagALL || tagNum >= numTags) error of some kind

  // Close all the windows
  for (ix1 = 0; ix1 < numlocales; ix1++) {
    if (theLocales[ix1].win != NULL) {
      theLocales[ix1].win->hide();
    }
    for (ix2 = 0; ix2 < numlocales; ix2++)  {
      if (comms[ix1][ix2].win != NULL) 
        comms[ix1][ix2].win->hide();
    }
  }

  // Select data to use
  curTagData = VisData.getTagData(tagNum);

  // Set the max values in the info bar
  Info->setMaxes(curTagData->maxTasks, curTagData->maxComms, curTagData->maxSize,
                 curTagData->maxCpu, curTagData->maxClock);

 }

static void selTag(Fl_Widget *w, void *p)
{
  long ix = (long) p;
  struct tagInfo *ptr = (struct tagInfo *)p;
  
  if (ix == DataModel::TagALL) {
    // printf ("selTag called on All\n");
    Info->setTagName("All");
    DbgView->selectData(ix);
  } else if (ix == DataModel::TagStart) {
    // printf ("selTag called on Start\n");
    Info->setTagName("Start");
    DbgView->selectData(ix);
  } else {
    // printf ("selTag called on tag %d \"%s\"\n", ptr->tagNo, ptr->tagName);
    Info->setTagName(ptr->tagName);
    DbgView->selectData(ptr->tagNo);
  }
  DbgView->redraw();
  Info->redraw();
}

void ViewField::makeTagsMenu(void)
{
  // Remove the old one if it exists
  if (tagMenu > 0) {
    MainMenuBar->remove(tagMenu);
    tagMenu = -1;
  }
  if (VisData.NumTags() >= 1) {

    // printf("Make tags menu, %d tags\n", VisData.NumTags());

    // Build the menu

    tagMenu = MainMenuBar->add("Tags", 0, 0, 0, FL_SUBMENU);
    if (tagMenu < 1) {
      printf ("Menu problem!\n");
      return;
    }
    MainMenuBar->add("Tags/All", 0, selTag, (void *)DataModel::TagALL);
    MainMenuBar->add("Tags/Start", 0, selTag, (void *)DataModel::TagStart);

    long ix;
    for (ix = 0; ix < VisData.NumTags(); ix++) {
      std::string tName = "Tags/tag " + std::to_string(ix) + " ("
                          + VisData.getTagName(ix) + ")";
      MainMenuBar->add(tName.c_str(), 0, selTag, (void *)&tags[ix], 0);
      tags[ix].tagNo = ix;
      tags[ix].tagName = strdup(tName.c_str());
      // printf ("Tag[%ld] is '%s'\n", ix, tags[ix].tagName);
    }
    MainMenuBar->redraw();
  }
}


void ViewField::drawLocale ( int ix, Fl_Color col)
{
  //printf ("drawLocale %d\n", ix);
    char ixchars[10];
    if (theLocales != NULL) {
      localeInfo *loc = &theLocales[ix];
      
      // Draw a locale ...
      loc->x = cx + (int) rint(rx * sin (angle * ix - start));
      loc->y = cy - (int) rint(ry * cos (angle * ix - start));
      snprintf (ixchars, 10, "%d", ix);
      loc->w = 30;
      loc->h = 30;
      
      fl_color(col);
      fl_rectf(loc->x-loc->w/2, loc->y-loc->h/2, loc->w, loc->h);
      fl_color(FL_BLACK);
      fl_rect(loc->x-loc->w/2, loc->y-loc->h/2, loc->w, loc->h);
      fl_draw(ixchars, loc->x+3-loc->w/2, loc->y+3-loc->h/2,
              loc->w-6, loc->h-6, FL_ALIGN_CENTER, NULL, 0);
    }
}


void ViewField::drawCommLine (int ix1, Fl_Color col1,  int ix2, Fl_Color col2)
{
  int x1, x2, y1, y2, dx, dy, midx, midy;
  if (ix1 < 0 || ix1 >= numlocales || ix2 < 0 || ix2 >= numlocales) {
    fprintf (stderr, "DATA ERROR: drawCommLine\n");
    return;
  }
  localeInfo *loc1 = &theLocales[ix1], *loc2 = &theLocales[ix2];
  double theta, cw, ch;
  
  dx = loc2->x - loc1->x;
  dy = loc2->y - loc1->y;
  if (dx == 0) {
    theta = (dy < 0 ? pi / 2 : pi * 1.5);
  } else {
    if (dx < 0) 
      theta = pi + atan((double)dy/(double)dx);
    else
      theta = atan((double)dy/(double)dx);
  }
  cw = loc1->w / sin(pi/4);
  ch = loc1->h / sin(pi/4);
  //printf ("dx = %d, dy = %d, Theta = %lf\n", dx, dy, theta);
  
  //  Need correct calculations here.
  x1 = rint (cw/2 * cos(theta));
  x1 = loc1->x + (abs(x1) > loc1->w/2 ? (dx < 0 ? -1 : 1)*loc1->w/2 : x1);
  y1 = rint (ch/2 * sin(theta));
  y1 = loc1->y + (abs(y1) > loc1->h/2 ? (dy < 0 ? -1 : 1)*loc1->h/2 : y1);

  x2 = rint (cw/2 * cos(pi+theta));
  x2 = loc2->x + (abs(x2) > loc2->w/2 ? (dx < 0 ? 1 : -1)*loc2->w/2 : x2);
  y2 = rint (ch/2 * sin(pi+theta));
  y2 = loc2->y + (abs(y2) > loc2->h/2 ? (dy < 0 ? 1 : -1)*loc2->h/2 : y2);

  midx = loc1->x + dx/2;
  midy = loc1->y + dy/2;

  if (col1 != FL_WHITE) {
    fl_color(col1);
    fl_line_style(FL_SOLID, 3, NULL);
    fl_line(x1,y1,midx,midy);
  }
  if (col2 != FL_WHITE) {
    fl_color(col2);
    fl_line_style(FL_SOLID, 3, NULL);
    fl_line(midx,midy,x2,y2);
  }
  fl_line_style(FL_SOLID, 1, NULL);
}

void ViewField::draw()
{
  //printf ("ViewField draw, numlocales is %d\n", numlocales);
  //printf ("draw: x,y = %d,%d, h,w = %d,%d\n", x(), y(), w(), h());

  cx = x() + w()/2;
  cy = y() + h()/2;
  rx = 0.85 * w() / 2;
  ry = 0.85 * h() / 2;

  Fl_Box::draw();

  int ix;
  for (ix = 0; ix < numlocales; ix++) {
    switch (infoTop) {
    case show_Tasks:
      drawLocale(ix, heatColor(curTagData->locales[ix].numTasks, curTagData->maxTasks));
      break;
    case show_CPU:
      drawLocale(ix, heatColor(curTagData->locales[ix].Cpu, curTagData->maxCpu));
      break;
    case show_Clock:
      drawLocale(ix, heatColor(curTagData->locales[ix].clockTime, curTagData->maxClock));
    }
  }

  int iy;
  for (ix = 0; ix < numlocales-1; ix++) {
    for (iy = ix + 1; iy < numlocales; iy++) {
      int  com2ix, com2iy, comMax; 
      if (showcomms) {
        com2ix = curTagData->comms[iy][ix].numComms;
        com2iy = curTagData->comms[ix][iy].numComms;
        comMax = curTagData->maxComms;
      } else {
        com2ix = curTagData->comms[ix][iy].commSize;
        com2iy = curTagData->comms[iy][ix].commSize;
        comMax = curTagData->maxSize;
      }
      if (com2ix || com2iy) {
        // Draw a line ... gray if no communication
        if (!com2ix || !com2iy) {
          drawCommLine(ix, com2ix ? heatColor(com2ix, comMax) : FL_GRAY,
                       iy, com2iy ? heatColor(com2iy, comMax) : FL_GRAY);
        } else {
          drawCommLine(ix, heatColor(com2ix, comMax),
                       iy, heatColor(com2iy, comMax));
        }
      }
    }
  }
  // Set up the info widget correctly
  if (showcomms)
    Info->showComms();
  else 
    Info->showSize();
  switch (infoTop) {
    case show_Tasks:
      Info->showTasks();
      break;
    case show_CPU:
      Info->showCpu();
      break;
    case show_Clock:
      Info->showClock();
      break;
  }
    
  Info->draw();
}

// Checks to see if click is on a comm link.
// Returns:  0 if not:  1 if closer to loc1, 2 if closer to loc2.
static int isOnCommLink ( int x, int y, localeInfo *loc1, localeInfo *loc2) {
  float slope;
  float newy;
  float ydiff;

  // Quick checks not between locations
  if ((x < loc1->x-3 && x < loc2->x-3)
      || (x > loc1->x+3 && x > loc2->x+3)
      || (y < loc1->y-3 && y < loc2->y-3)
      || (y > loc1->y+3 && y > loc2->y+3)) {
    return 0;
  }

  int ylen = loc2->y - loc1->y;
  int xlen = loc2->x - loc1->x;

  if (xlen == 0) {
    // Vertically above each other
    int xdiff = abs(loc2->x - x);
    // printf ("Vertically above each other. xdiff = %d\n", xdiff);
    if (xdiff <= 3) {
      if (abs(loc2->y - y) > abs(loc1->y - y))
        return 1;
      else
        return 2;
    }
  } else {
    slope =  (float)(ylen) / (float)(xlen);
    if (fabs(slope) < 2) {
      newy  = (float)loc2->y - slope * (loc2->x - x);
      ydiff = fabs(y - newy);
      // printf ("slope: %f ydiff = %f\n", slope, ydiff);
      if (ydiff < 5) {
        // assumed on the line
        if (abs(loc2->x - x) < abs(xlen)/2)
          return 2;
        else
          return 1;  
      }
    } else {
      float newx  = (float)loc2->x - (loc2->y - y)/slope;
      float xdiff = fabs(x - newx);
      // printf ("slope: %f xdiff = %f\n", slope, xdiff);
      if (xdiff < 5) {
        // assumed on the line
        if (abs(loc2->y - y) < abs(ylen)/2)
          return 2;
        else
          return 1;  
      }
    }
  }

  return 0;
}

int ViewField::handle(int event)
{
  int ix;

  int x = Fl::event_x();
  int y = Fl::event_y();

  int i, j; // search for the comm link clicked
  
  switch (event) {
  case FL_PUSH:
    //printf ("Push at (%d,%d)\n", x, y);
    break;
  case FL_RELEASE:
    //printf ("Release at (%d,%d)\n", x, y);
    // Click on a locale?
    if (numlocales > 0) {
      for (ix = 0; ix < numlocales; ix++) {
        // See if release is inside a locale
        localeInfo *loc = &theLocales[ix];
        if ( x > loc->x-loc->w/2 && x <= loc->x + loc->w/2 &&
             y > loc->y-loc->h/2 && y <= loc->y + loc->h/2) {
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
          return 1;
        }
      }
    }

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
    
    break;
  }
  return Fl_Box::handle(event);
}

