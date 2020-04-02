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


#include "chplvis.h"

#include <FL/fl_draw.H>
#include <FL/x.H>

#include <string.h>

#ifdef __APPLE__
static const int Y_OFFSET = 10;
#else
static const int Y_OFFSET = 0;
#endif

static const int CR_Left = 25;

static const int LC_Box_Left = 280;
static const int LC_Box_W = 200;
static const int LC_Box_H = 80;

// Local utility functions

// Computation of color for use in displays

Fl_Color heatColor(double val, double max) {
  SettingsData *s = &VisSettings;
  if (val == 0) return FL_WHITE;
  if (max == 1) return fl_rgb_color(s->maxR, s->maxG, s->maxB);
  // if (val == 1) return fl_rgb_color(0,180,255);
  return fl_rgb_color( s->minR + (s->maxR-s->minR)*(val/max),
                       s->minG + (s->maxG-s->minG)*(val/max),
                       s->minB + (s->maxB-s->minB)*(val/max));
}

// XXX should new and delete here?
void InfoBar::setFileName(const char *name) {
    char *sl;
    char *dash;
    int nameLen = strlen(name);
    char tmpNam[nameLen+1];

    strncpy (tmpNam, name, nameLen+1);
    if (tmpNam[nameLen-1] == '/') {
      tmpNam[nameLen-1] = 0;
      nameLen--;
    }
    sl = strrchr(tmpNam,'/');
    if (sl == NULL)
      sl = tmpNam;
    else
      sl++;
    dash = strrchr(sl,'-');
    if (dash != NULL) *dash = 0;
    if (fileName != NULL) free(fileName);
    fileName = strdup(sl);
  }

void InfoBar::setTagName(const char *name) {
    const char *sl;
    if (tagName != NULL) free(tagName);
    sl = strchr(name, '/');
    if (sl == NULL)
      sl = name;
    else
      sl++;
    tagName = strdup(sl);
  }

void InfoBar::draw(void)
{
  char mesg[150] = "";

  // dimensions and locations
  int cr_h = h() / 3;               // Height of color reference
  int cr_y = y() + 30 + Y_OFFSET;        // Y of color reference

  Fl_Group::draw();

  if (showcolorref) {
    // Color Reference
    int ix;
    for (ix = 0; ix <= 30; ix++) {
      fl_color(heatColor(ix+1,31));
      fl_line_style(FL_SOLID,3,NULL);
      fl_line(x()+CR_Left+3*ix,cr_y,x()+CR_Left+3*ix,cr_y+cr_h);
      if ((ix%3) == 0) {
        fl_color(FL_BLACK);
        fl_line_style(FL_SOLID,1,NULL);
        fl_line(x()+CR_Left+3*ix, cr_y+cr_h+2, x()+CR_Left+3*ix, cr_y+cr_h+6);
      }
    }

    fl_color(FL_BLACK);
    fl_line_style(FL_SOLID,1,NULL);
    fl_draw("1", x()+CR_Left-20, cr_y, 15, 30, FL_ALIGN_CENTER, NULL, 0);

    // Messages on max counts, next to the color reference
    switch (infoTop) {
    case show_Tasks:
      if (maxTasks > 0)
        snprintf (mesg, 150, "max Tasks: %d", maxTasks);
      break;
    case show_CPU:
      if(maxCpu > 0)
        snprintf (mesg, 150, "max CPU: %f", maxCpu);
      break;
    case show_Clock:
      if (maxClock > 0)
        snprintf (mesg, 150, "max Clock: %f", maxClock);
      break;
    case show_Concurrency:
      if (maxClock > 0)
        snprintf (mesg, 150, "max Concurrent: %ld", maxConcurrent);
      break;
    }
    fl_draw(mesg, x()+CR_Left+100, cr_y, 120, 20, FL_ALIGN_LEFT, NULL, 0);

    mesg[0] = 0;
    if (showcomms) {
      if (maxComms > 0)
        snprintf (mesg, 150, "max Comms: %d", maxComms);
    } else {
      if (maxSize > 0)
        snprintf (mesg, 150, "max Data: %ld", maxSize);
    }
    fl_draw(mesg, x()+CR_Left+100, cr_y+cr_h/2, 120, 20,FL_ALIGN_LEFT, NULL, 0);
  }

  // Messages about file names / tags and so forth

  if (fileName != NULL) {
    snprintf (mesg, 150, "file: %s", fileName);
    fl_draw(mesg, x()+5, y()+5+Y_OFFSET, 120, 20, FL_ALIGN_LEFT, NULL, 0);
  }

  if (showtag && tagName != NULL) {
    snprintf (mesg, 150, "tag: %s", tagName);
    fl_draw(mesg, x()+130, y()+5+Y_OFFSET, 120, 20, FL_ALIGN_LEFT, NULL, 0);
  }

}

bool InfoBar::isOnList (LocCommBox *box)
{
  std::list<LocCommBox *>::iterator itr;
  itr = infoBoxes.begin();
  while (itr != infoBoxes.end()) {
    if ((*itr)->boxKind() == box->boxKind()) {
      switch (box->boxKind()) {
        case LocCommBox::I_Locale:
          if ((*itr)->getLoc() == box->getLoc())
            return true;
          break;
        case LocCommBox::I_Comm:
          {
            int L1, L2;
            int B1, B2;
            (*itr)->getCommLocs(L1, L2);
            box->getCommLocs(B1, B2);
            if (L1 == B1 && L2 == B2)
              return true;
          }
          break;
        default: // Nothing
          break;
      }
    }
    itr++;
  }
    return false;
}

void InfoBar::addLocOrComm(LocCommBox *box)
{
  // don't double add
  if (isOnList(box)) {
    boxCache.insert(boxCache.begin(), box);
    return;
  }

  // Move others over or remove them
  int newW = box->w();

  std::list<LocCommBox *>::iterator itr;

  itr = infoBoxes.begin();
  while (itr != infoBoxes.end()) {
    if ((*itr)->x()+newW+(*itr)->w() < x()+w()) {
      (*itr)->position((*itr)->x()+newW, (*itr)->y());
      (*itr)->redraw();
      itr++;
    } else {
      LocCommBox *dbox = *itr;
      remove(*itr);
      itr = infoBoxes.erase(itr);
      boxCache.insert(boxCache.begin(), dbox);
    }
  }

  box->position(x()+LC_Box_Left, y()+Y_OFFSET);
  //printf ("infoBoxes size %ld\n", infoBoxes.size());
  add(box);
  //printf ("Added box 0x%lx\n", (long)box);
  infoBoxes.insert(infoBoxes.begin(), box);
  MainWindow->redraw();
}

void InfoBar::delLocOrComm(LocCommBox *box)
{
  // printf ("delLocOrComm ...\n");
  remove(box);
  infoBoxes.remove(box);
  //printf ("Removed box 0x%lx\n", (long)box);
  //printf ("infoBoxes size %ld\n", infoBoxes.size());
  std::list<LocCommBox *>::iterator itr = infoBoxes.begin();
  int x = LC_Box_Left;
  while (itr != infoBoxes.end()) {
    (*itr)->resize(x, (*itr)->y(), LC_Box_W, LC_Box_H);
    x += (*itr)->w();
    itr++;
  }
  boxCache.insert(boxCache.begin(), box);
  MainWindow->redraw();
}

LocCommBox * InfoBar::getNewLocComm()
  {
    if (boxCache.empty()) {
      return new LocCommBox(0, 0, LC_Box_W, LC_Box_H);
    } else {
      std::list<LocCommBox *>::iterator itr = boxCache.begin();
      boxCache.erase(itr);
      return *itr;
    }
  }

void InfoBar::rmAllLocOrComm(void)
  {
     std::list<LocCommBox *>::iterator itr = infoBoxes.begin();
     LocCommBox *box;

     while (itr != infoBoxes.end()) {
       box = *itr;
       itr = infoBoxes.erase(itr);
       remove(box);
       boxCache.insert(boxCache.begin(), box);
     }
  }

void InfoBar::resize (int X, int Y, int W, int H)
  {
    // Don't resize the children!
    Fl_Widget::resize(X, Y, W, H);
  }
