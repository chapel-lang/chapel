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

#include <stdio.h>

#include "LocaleWin.h"
#include "ViewField.h"

LocaleWin::LocaleWin (int x, int y, int W, int H, const char *l)
  :  Fl_Double_Window (W, H, l)
{
}

void LocaleWin::updateWin(localeData *l)
{
  //  Write the data to the window
  const int msgsize = 1024;
  char mesg[msgsize];
  snprintf (mesg, msgsize, "Locale %d", locnum);
  title->copy_label(mesg);
  
  // Create the text
  loc = l;
  snprintf (mesg, msgsize, "Number of Tasks: %ld\n"
            "CPU:  User %lf\n"
            "      Sys %lf\n"
            "      Total %lf\n"
            "Clock: %lf\n",
            loc->numTasks,
            loc->userCpu,
            loc->sysCpu,
            loc->Cpu,
            loc->clockTime);
  info->value(mesg);
}
