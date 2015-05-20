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

static const int CR_Left = 220;

#ifdef __APPLE__
static const int Y_OFF = 10;
#else
static const int Y_OFF = 0;
#endif

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
  char mesg[150];
  if (maxTasks > 0) {
    if (showtasks)
      snprintf (mesg, 150, "max Tasks: %d", maxTasks);
    else
      snprintf (mesg, 150, "max CPU: %f", maxCpu);
    fl_draw(mesg, x()+CR_Left+105, y_center-20, 120, 20, FL_ALIGN_LEFT, NULL, 0);
    if (showcomms)
      snprintf (mesg, 150, "max Comms: %d", maxComms);
    else
      snprintf (mesg, 150, "max Data: %ld", maxSize);
    fl_draw(mesg, x()+CR_Left+105, y_center, 120, 20, FL_ALIGN_LEFT, NULL, 0);
  }

  // Messages about file names / tags and so forth
}


