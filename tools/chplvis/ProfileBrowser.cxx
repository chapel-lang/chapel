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
  Fl_Browser(x,y,w,h,l)
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

int ProfileBrowser::handle (int event)
{
  int myY = Fl::event_y();

  if (event == FL_PUSH || event == FL_DRAG) {
    void *sel = lastSelected;
    void *itm = find_item(myY);
    if (itm != sel) {
      // printf ("itm is 0x%ld, sel is 0x%ld\n", (long)itm, (long)sel);
      if (sel != NULL) {
        item_select(sel,0);
        redraw_line(sel);
      }
      item_select(itm,1);
      redraw_line(itm);
      lastSelected = itm;
    } else if (event == FL_PUSH) {
      printf ("Should unselect item\n");
      item_select(sel,0);
      redraw_line(sel);
      lastSelected = NULL;
    }
  }
  
  int rv = Fl_Browser::handle(event);

#if 0  
  // printf ("PB::handle: ev = %s, rv = %d\n", fl_eventnames[event], rv);
  if (rv == 1 && event == FL_RELEASE) {
    if (lastSelected != NULL) {
      item_select(lastSelected,1);
      redraw_line(lastSelected);
      printf ("release new selected.\n");
    }
    // select(lastSelected);
    // redraw();
    // printf("selected row %d\n", lastSelected);
  }
#endif
  
  return rv;

}

void ProfileBrowser:: showFileFor(int index)
{
  printf ("should show file for function %s\n", funcInfo[index-1]->name);
}
