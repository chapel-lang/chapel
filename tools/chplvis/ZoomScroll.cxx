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

#include "ZoomScroll.h"
#include "chplvis.h"     // Access to the main window for redrawing

void ZoomScroll::resize (int new_x, int new_y, int new_w, int new_h)
{
  // Do nothing if zoomWidget not set yet.
  if (!zoomWidget) {
    Fl_Scroll::resize(new_x,new_y,new_w,new_h);
    return;
  }
  
  int dbv_x = zoomWidget->x();
  int dbv_y = zoomWidget->y();
  int dbv_w = zoomWidget->w();
  int dbv_h = zoomWidget->h();
  int my_x = x();
  int my_y = y();
  int my_w = w();
  int my_h = h();

  // Testing ....
  //printf ("ZoomScroll resize (%d,%d,%d,%d), dbg (%d,%d,%d,%d), my (%d, %d, %d,%d)\n",
  // new_x, new_y, new_w, new_h, dbv_x, dbv_y, dbv_w, dbv_h, my_x, my_y, my_w, my_h);

  // Adjust dbv's x and y
  if ((dbv_x < my_x) && dbv_x+dbv_w < new_x+new_w) {
    // adjust x
    dbv_x = new_x + new_w - dbv_w;
    if (dbv_x > new_x)
      dbv_x = new_x;
  }
  if ((dbv_y < my_y) && dbv_y+dbv_h < new_y+new_h) {
    // adjust y
    dbv_y = new_y + new_h - dbv_h;
    if (dbv_y > new_y)
      dbv_y = new_y;
  }

  if ((dbv_w-5 <= my_w) && (dbv_h-5 <= my_h))
    zoomWidget->resize(new_x, new_y, new_w, new_h);
  else if ((dbv_w-5 <= my_w) && (new_w > my_w))
    zoomWidget->resize(new_x, dbv_y, new_w, dbv_h);
  else if ((dbv_h-5 <= my_h) && (new_h > my_h))
    zoomWidget->resize(dbv_x, new_y, dbv_w, new_h);
  else
    zoomWidget->resize(dbv_x, dbv_y, dbv_w, dbv_h);

  Fl_Scroll::resize(new_x,new_y,new_w,new_h);
}


void ZoomScroll::zoomIn(int zx, int zy)
{
  // Do nothing if zoomWidget not set yet.
  if (!zoomWidget) return;
  
  // Multiply size by 1.2, center of scroll to stay fixed.
  zoomWidget->resize((12*zoomWidget->x() - 2*(x() + w()/2))/10,
                  (12*zoomWidget->y() - 2*(y() + h()/2))/10,
                  (zoomWidget->w()*12)/10,
                  (zoomWidget->h()*12)/10);
  MainWindow->redraw();
}

void ZoomScroll::zoomOut(int zx, int zy)
{
  // Do nothing if zoomWidget not set yet.
  if (!zoomWidget) return;

  int new_w = (zoomWidget->w()*8)/10;
  int new_h = (zoomWidget->h()*8)/10;
  int new_x = (8*zoomWidget->x() + 2 * (x() + w()/2))/10;
  int new_y = (8*zoomWidget->y() + 2 * (y() + h()/2))/10;
  if (new_x > x())
    new_x = x();
  if (new_y > y())
    new_y = y();
  if (new_w < w())
     new_w = w();
  if (new_h < h())
    new_h = h();
  zoomWidget->resize(new_x, new_y, new_w, new_h);
  MainWindow->redraw();
}

void ZoomScroll::reset(void)
{
  // Do nothing if zoomWidget not set yet.
  if (!zoomWidget) return;

  zoomWidget->resize(x(),y(),w(),h());
  MainWindow->redraw();
}
