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


#include "InfoBar.h"
#include "ViewField.h"  // To get access to heatColor

#include <FL/fl_draw.H>
#include <FL/x.H>

#include <string.h>

static const int CR_Left = 220;

#ifdef __APPLE__
static const int Y_OFF = 10;
#else
static const int Y_OFF = 0;
#endif

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
  // dimensions and locations
  int y_center = y() + (h()-Y_OFF) / 2;  // Center line of infobar
  int cr_h = (h()-Y_OFF) / 2;               // Height of color reference
  int cr_y = y() + cr_h / 2;        // Y of color reference

  Fl_Box::draw();

  // Separation line between InfoBar and the display field
  
  fl_color(FL_BLACK);
  fl_line_style(FL_SOLID, 2, NULL);
  fl_line(x(),y()+h()-Y_OFF,x()+w(),y()+h()-Y_OFF);

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
  fl_draw("1", x()+CR_Left-20, y_center-20, 15, 40, FL_ALIGN_CENTER, NULL, 0);
  //  fl_draw("Max", x()+CR_Left+100, y()+5, 15, h()-10, FL_ALIGN_CENTER, NULL, 0);

  // Messages on max counts, next to the color reference
  char mesg[150] = "";
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
    case show_Concurrent:
      if (maxClock > 0)
        snprintf (mesg, 150, "max Concurrent: %ld", maxConcurrent);
      break;
  }

  fl_draw(mesg, x()+CR_Left+105, y_center-20, 120, 20, FL_ALIGN_LEFT, NULL, 0);
  mesg[0] = 0;
  if (showcomms) {
    if (maxComms > 0) 
      snprintf (mesg, 150, "max Comms: %d", maxComms);
  } else {
    if (maxSize > 0)
      snprintf (mesg, 150, "max Data: %ld", maxSize);
  }
  fl_draw(mesg, x()+CR_Left+105, y_center, 120, 20, FL_ALIGN_LEFT, NULL, 0);


  // Messages about file names / tags and so forth

  if (fileName != NULL) {
    snprintf (mesg, 150, "file: %s", fileName);
    fl_draw(mesg, x()+5, y_center-20, 120, 20, FL_ALIGN_LEFT, NULL, 0);
  }

  if (tagName != NULL) {
    snprintf (mesg, 150, "tag: %s", tagName);
    fl_draw(mesg, x()+5, y_center, 120, 20, FL_ALIGN_LEFT, NULL, 0);
  }

}


