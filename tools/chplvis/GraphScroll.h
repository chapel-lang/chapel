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

#ifndef GRAPHSCROLL_H
#define GRAPHSCROLL_H

#include <FL/Fl_Scroll.H>

class GraphScroll : public Fl_Scroll {

 public:

  GraphScroll (int x, int y, int w, int h, const char *l = 0)
    : Fl_Scroll::Fl_Scroll (x, y, w, h, l) {}; 
    

  void resize (int, int, int, int);

  void zoomIn(void);
  void zoomOut(void);
  void reset(void);

};

#endif
