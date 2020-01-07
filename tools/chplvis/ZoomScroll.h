/*
 * Copyright 2016-2020 Cray Inc.
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

#ifndef ZOOMSCROLL_H
#define ZOOMSCROLL_H

#include <FL/Fl_Scroll.H>

class ZoomScroll : public Fl_Scroll {

  Fl_Widget *zoomWidget;

 public:

  ZoomScroll (int x, int y, int w, int h, const char *l = 0)
    : Fl_Scroll::Fl_Scroll (x, y, w, h, l) { zoomWidget = 0; }; 

  void setZoomWidget (Fl_Widget *zw) { zoomWidget = zw; }
    
  Fl_Widget *getZoomWidget (void) { return zoomWidget; }
    
  void resize (int, int, int, int);

  void zoomIn(int zx = -1, int zy=-1);
  void zoomOut(int zx = -1, int zy=-1);
  void reset(void);

};

#endif
