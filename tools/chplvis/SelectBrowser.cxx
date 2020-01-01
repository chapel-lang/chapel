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

#include "SelectBrowser.h"
#include <string.h>

SelectBrowser::SelectBrowser (int x, int y, int w, int h, const char *l) :
  Fl_Browser(x,y,w,h,l)
{
  lastSelected = NULL;
};

int SelectBrowser::handle (int event)
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
      if (itm != NULL) {
        item_select(itm,1);
        redraw_line(itm);
      }
      lastSelected = itm;
    } else if (event == FL_PUSH && sel != NULL) {
      item_select(sel,0);
      redraw_line(sel);
      lastSelected = NULL;
    }
  }
  
  int rv = Fl_Browser::handle(event);
  
  return rv;

}

