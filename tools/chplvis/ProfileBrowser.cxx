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

void ProfileBrowserCB (Fl_Widget *w, void *p)
{
  // printf ("ProfileBrowser p %ld\n", (long)p);
  for (int ix = 1; ix <=  VisData.numFunctionNames(); ix++ ) {
    if (ProfileView->selected(ix)) {
      //printf ("selected %d", ix);
      ProfileView->showFileFor(ix);
    }
  }
}

int compTask (const void *p1, const void *p2)
{
  funcname *a = *(funcname **)p1;
  funcname *b = *(funcname **)p2;
  return  b->noTasks - a->noTasks; 
}

ProfileBrowser::ProfileBrowser (int x, int y, int w, int h, const char *l) :
  SelectBrowser(x,y,w,h,l)
{
  static int width[] = {10, 60, 250, 0 };
  column_widths(width);
  initialized = false;
  lastSelected = NULL;
};

void ProfileBrowser::loadData()
{
  callback ((Fl_Callback*)ProfileBrowserCB);
  numFuncs = VisData.numFunctionNames();
  funcInfo = (const funcname**) new funcname*[numFuncs];
  for (int ix = 0; ix < numFuncs; ix++) {
    funcInfo[ix] = VisData.getFunctionInfo(ix);
  }
}

void ProfileBrowser::prepareData()
{
  if (!initialized) {
    qsort (funcInfo, numFuncs, sizeof(funcname*), compTask);
    for (int ix = 0; ix < numFuncs; ix++) {
      char line[512];
       const char *fName = VisData.fileName(funcInfo[ix]->fileNo);
      snprintf (line, sizeof(line), "\t@r%d\t%s\t%s:%d",
                funcInfo[ix]->noTasks,
                funcInfo[ix]->name,
                (fName[0] == '$' ? &fName[11] : fName),
                funcInfo[ix]->lineNo);
      add(line);
    }
    initialized = true;
  }
}

void ProfileBrowser:: showFileFor(int index)
{
  printf ("should show file for function %s\n", funcInfo[index-1]->name);
  SubView *show = new SubView(x(),y(),w(),h());
  const char *fname = VisData.fileName(funcInfo[index-1]->fileNo);
  char text[512];
  snprintf (text, sizeof(text), "Function %s defined by file %s.",
            funcInfo[index-1]->name, (fname[0] == '$' ? &fname[11] : fname));
  show->headerText (text);
  show->showBackButton();

  // Make correct file name.
  if (fname[0] == '$') {
    const char *home = VisData.CHPL_HOME();
    printf ("Opening file: '%s/%s'\n",  home, &fname[11]);
    snprintf (text, sizeof(text), "%s/%s", home, &fname[11]);
  } else {
    const char *dir = VisData.DIR();
    snprintf (text, sizeof(text), "%s/%s", dir, fname);
  }
  if (!show->ShowFile(text, funcInfo[index-1]->lineNo))
    return;

  // Build the file browser

  DataField->pushNewChild(show,true);
}
