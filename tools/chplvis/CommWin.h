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

#ifndef COMMWIN_H
#define COMMWIN_H

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Multiline_Output.H>
struct commInfo;

class CommWin : public Fl_Double_Window {

 private:
  Fl_Box *title;
  Fl_Multiline_Output *info;
  commInfo *comm;
  int fromLoc, toLoc;

 public:
  CommWin (int x, int y, int W, int H, const char *l=0);

  void setMembers (Fl_Box *t, Fl_Multiline_Output *i, commInfo *c, int fromLn, int toLn) {
    title = t;
    info = i;
    comm = c;
    fromLoc = fromLn;
    toLoc = toLn;
  }

  void updateWin ();
    
};

#endif
