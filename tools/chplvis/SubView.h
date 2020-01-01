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

#ifndef SUBVIEW_H
#define SUBVIEW_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>

#include "DataModel.h"

// General class that has the top row of an optional back
// button and a view that fills the rest of the view.
// Typically this is some line oriented browser, like  a
// "file view" or a list of comms for a particular locale.

class SubView : public Fl_Group {

  Fl_Button *backBtn;
  Fl_Button *title;
  Fl_Widget *body;

  // For taskcomm
  taskData *dispTask;

  public:

  SubView (int x, int y, int w, int h, const char *l = 0);

  ~SubView();

  void showBackButton (void (*cb)(Fl_Widget *));  // also register a callback

  void showBackButton (void);                     // Use a standard callback

  void headerText (const char *t);

  bool ShowFile (const char *filename, int lineNo);

  bool ShowTaskComm (taskData *task);
  void TaskCommCB (void);

};


#endif
