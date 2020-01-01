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

/* Implementation of a "Multi-group", similar to a Tab set but without
 * the tabs.  Control of which group is visible is one of several methods
 *  that select the group to be displayed.
 */

#include "DataModel.h"
#include "ProfileBrowser.h"
#include "SubView.h"
#include "chplvis.h"
#include <string.h>
#include <stdlib.h>
#include <FL/names.h>
#include <FL/fl_ask.H>

void ProfileBrowserCB (Fl_Widget *w, void *p)
{
  // printf ("ProfileBrowser p %ld\n", (long)p);
  if (ProfileView->selected(1)) {
    ProfileView->select(1,0);
  }
  for (int ix = 2; ix <=  VisData.numFunctionNames()+1; ix++ ) {
    if (ProfileView->selected(ix)) {
      //printf ("selected %d", ix);
      ProfileView->showFileFor(ix-2);
    }
  }
}

static int longWidth[] = {10, 85, 250, 0 };
static int doubleWidth[] = {10, 100, 250, 0};

int compTask (const void *p1, const void *p2)
{
  funcInfo *a = *(funcInfo **)p1;
  funcInfo *b = *(funcInfo **)p2;
  return  b->noTasks - a->noTasks; 
}

int compClockTime (const void *p1, const void *p2)
 {
  funcInfo *a = *(funcInfo **)p1;
  funcInfo *b = *(funcInfo **)p2;
  return (b->clockTime > a->clockTime ? 1 :
          (b->clockTime < a->clockTime ? -1 : 0));
}

int compComm (const void *p1, const void *p2)
{
  funcInfo *a = *(funcInfo **)p1;
  funcInfo *b = *(funcInfo **)p2;
  return  (b->noGets + b->noPuts +  b->noOnTasks )
          - (a->noGets + a->noPuts +  a->noOnTasks );
}

int compGets (const void *p1, const void *p2)
{
  funcInfo *a = *(funcInfo **)p1;
  funcInfo *b = *(funcInfo **)p2;
  return  b->noGets - a->noGets;
}

int compPuts (const void *p1, const void *p2)
{
  funcInfo *a = *(funcInfo **)p1;
  funcInfo *b = *(funcInfo **)p2;
  return  b->noPuts - a->noPuts;
}

int compOnCalls (const void *p1, const void *p2)
{
  funcInfo *a = *(funcInfo **)p1;
  funcInfo *b = *(funcInfo **)p2;
  return  b->noOnTasks - a->noOnTasks;
}



ProfileBrowser::ProfileBrowser (int x, int y, int w, int h, const char *l) :
  SelectBrowser(x,y,w,h,l)
{
  lastSelected = NULL;
};

void ProfileBrowser::loadData()
{
  callback ((Fl_Callback*)ProfileBrowserCB);
  numFuncs = VisData.numFunctionNames();
  funcList = (const funcInfo**) new funcInfo*[numFuncs];
  for (int ix = 0; ix < numFuncs; ix++) {
    funcList[ix] = VisData.getFunctionInfo(ix);
  }
}

void ProfileBrowser::prepareData(whichData what)
{
  const char *dataName;
  char line[1024];

  // Get rid of any current data
  clear();

  // Sort the data
  switch (what) {
    case D_CLOCK:
      qsort (funcList, numFuncs, sizeof(funcInfo*), compClockTime);
      dataName = "Clock";
      break;
    case D_TASKS:
      qsort (funcList, numFuncs, sizeof(funcInfo*), compTask);
      dataName = "Tasks";
      break;
    case D_COMM:
      qsort (funcList, numFuncs, sizeof(funcInfo*), compComm);
      dataName = "Comms";
      break;
    case D_ONCALLS:
      qsort (funcList, numFuncs, sizeof(funcInfo*), compOnCalls);
      dataName = "On calls";
      break;
    case D_GETS:
      qsort (funcList, numFuncs, sizeof(funcInfo*), compGets);
      dataName = "Gets";
      break;
    case D_PUTS:
      qsort (funcList, numFuncs, sizeof(funcInfo*), compPuts);
      dataName = "Puts";
      break;
  }

  // Prep browser
  if (what > D_CLOCK)
    column_widths(longWidth);
  else
    column_widths(doubleWidth);

  // Header line
  snprintf (line, sizeof(line),
      "@B10\t@r@b@m@B10%s\t@b@m@B10Internal Function Name\t@b@m@B10File:line",
      dataName);
  add(line);

  for (int ix = 0; ix < numFuncs; ix++) {
    long data = 0;
    const char *fName = VisData.fileName(funcList[ix]->fileNo);
    if (what == D_CLOCK) {
      snprintf (line, sizeof(line), "\t@r%.4lf\t%s\t%s:%ld",
                funcList[ix]->clockTime,
                funcList[ix]->name,
                (fName[0] == '$' ? &fName[11] : fName),
                funcList[ix]->lineNo);
    } else {
      switch (what) {
        case D_CLOCK:  // Should not happen, here to quiet compiler
          break;
        case D_TASKS:
          data = funcList[ix]->noTasks;
          break;
        case D_COMM:
          data = funcList[ix]->noOnTasks +  funcList[ix]->noGets
            +  funcList[ix]->noPuts;
            break;
        case D_ONCALLS:
          data = funcList[ix]->noOnTasks;
          break;
        case D_GETS:
          data = funcList[ix]->noGets;
          break;
        case D_PUTS:
          data = funcList[ix]->noPuts;
          break;
      }
      snprintf (line, sizeof(line), "\t@r%ld\t%s\t%s:%ld",
                data,  funcList[ix]->name,
                (fName[0] == '$' ? &fName[11] : fName),
                funcList[ix]->lineNo);
    }
    // Need to do this based on sort data
    if ((what == D_CLOCK && funcList[ix]->clockTime != 0)
        || (what > D_CLOCK && data != 0))
      add(line);
  }
}

// Index here is 0 based for list of files.

void ProfileBrowser:: showFileFor(int index)
{
  char text[512];
  // printf ("should show file for function %s\n", funcList[index-1]->name);
  SubView *show = new SubView (x(),y(),w(),h());
  const char *fname = VisData.fileName (funcList[index]->fileNo);
  snprintf (text, sizeof (text), "Function %s defined by file %s.",
            funcList[index]->name, (fname[0] == '$' ? &fname[11] : fname));
  show->headerText (text);
  show->showBackButton ();
  if (!show->ShowFile (fname, funcList[index]->lineNo)) {
    delete show; 
    return;
  }

  DataField->pushNewChild (show,true);
}
