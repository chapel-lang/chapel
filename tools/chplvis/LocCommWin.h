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

#ifndef LOCALEWIN_H
#define LOCALEWIN_H

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Multiline_Output.H>
#include "LocCommBox.h"

struct localeData;
struct commData;

// Window for showing Locale information.

class LocCommWin : public Fl_Double_Window {

 private:
  LocCommBox *LCBox;
  

 public:
  LocCommWin (int x, int y, int W, int H, const char *l=0);

  void setBox (LocCommBox *b) { LCBox = b; }

  void setAsLocale (int Loc, localeData *l);

  void setAsComm(int L1, int L2, commData *c);
  
};

#endif
