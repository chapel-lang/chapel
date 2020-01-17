/*
 * Copyright 2016-2020 Hewlett Packard Enterprise Development LP
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

#include <stdio.h>

#include "LocCommWin.h"
#include "GraphView.h"

LocCommWin::LocCommWin (int x, int y, int W, int H, const char *l)
  :  Fl_Double_Window (W, H, l)
{
  box(FL_BORDER_FRAME);
}

void LocCommWin::setAsLocale(int num, localeData *l)
{
  LCBox->setLocale(num, l);
  redraw();
}


void LocCommWin::setAsComm(int L1, int L2, commData *c)
{
  LCBox->setComm(L1, L2, c);
  redraw();
}

