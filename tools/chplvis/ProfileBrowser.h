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

#ifndef PROFILEBROWSER_H
#define PROFILEBROWSER_H

#include "SelectBrowser.h"
#include <vector>

class ProfileBrowser : public SelectBrowser {

  const funcInfo ** funcList;
  int numFuncs;
  void *lastSelected;

  public:

  enum whichData { D_CLOCK, D_TASKS, D_COMM, D_ONCALLS, D_GETS, D_PUTS };

  ProfileBrowser (int x, int y, int w, int h, const char *l = 0);

  void loadData();

  void prepareData(whichData what);

  void showFileFor(int ix);
  
};


#endif
