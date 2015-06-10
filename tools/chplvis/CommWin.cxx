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

#include "CommWin.h"
#include "ViewField.h"

CommWin::CommWin (int x, int y, int W, int H, const char *l)
  :  Fl_Double_Window (W, H, l)
{
}

void CommWin::updateWin()
{
  //  Write the data to the window
  const int msgsize = 1024;
  char mesg[msgsize];
  snprintf (mesg, msgsize, "Comm %d->%d", fromLoc, toLoc);
  title->copy_label(mesg);
  
  // Create the text
  snprintf (mesg, msgsize, "Total Comms = %d\nTotal bytes = %ld\n"
	    "  Gets = %d\n  Puts = %d\n  Forks = %d\n",
	    comm->numComms, comm->commSize, comm->numGets, comm->numPuts,
	    comm->numComms - comm->numGets - comm->numPuts);
  info->value(mesg);
}
