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

void InfoBar::draw(void)
{
  Fl_Box::draw();

  fl_color(FL_BLACK);
  fl_line_style(FL_SOLID, 2, NULL);
  fl_line(x(),y()+h(),x()+w(),y()+h());

  // Color Reference
  int ix;
  for (ix = 0; ix < 30; ix++) {
    fl_color(heatColor(ix+1,30));
    fl_line_style(FL_SOLID,3,NULL);
    fl_line(x()+40+3*ix,y()+5,x()+40+3*ix,y()+h()-10);
    if ((ix%3) == 0 && ix != 0) {
      fl_color(FL_BLACK);
      fl_line_style(FL_SOLID,1,NULL);
      fl_line(x()+40+3*ix, y()+h()-9, x()+40+3*ix, y()+h()-5);
    }
  }
  fl_color(FL_BLACK);
  fl_line_style(FL_SOLID,1,NULL);
  fl_draw("1", x()+10, y()+5, 15, h()-10, FL_ALIGN_CENTER, NULL, 0);
  fl_draw("Max", x()+140, y()+5, 15, h()-10, FL_ALIGN_CENTER, NULL, 0);

  // Messages on max counts
  char mesg[150];
  if (maxTasks > 0) {
    if (showtasks)
      snprintf (mesg, 150, "maxTasks: %d", maxTasks);
    else
      snprintf (mesg, 150, "maxCPU: %f", maxCpu);
    fl_draw(mesg, x()+180, y()+5, 120, h()-10, FL_ALIGN_LEFT, NULL, 0);
    if (showcomms)
      snprintf (mesg, 150, "maxComms: %d", maxComms);
    else
      snprintf (mesg, 150, "maxData: %ld", maxSize);
    fl_draw(mesg, x()+330, y()+5, 120, h()-10, FL_ALIGN_LEFT, NULL, 0);
  }
}


