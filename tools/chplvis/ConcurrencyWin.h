/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2015-2019 Cray Inc.
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

#ifndef CONCURRENCYWIN_H
#define CONCURRENCYWIN_H

class ConcurrencyWin; 

#include <FL/Fl_Double_Window.H>
#include "ConcurrencyView.h"

// Window for showing concurrency details.

class ConcurrencyWin : public Fl_Double_Window {

  private:
    ConcurrencyView *dataView;

  public:
    ConcurrencyWin (int x, int y, int W, int H, const char *l=0);

    void setMembers (ConcurrencyView *v) {
       dataView = v;
    }

    void updateData (long loc, long tag) { dataView->updateData(loc,tag); }

    void showCommListFor(taskData *task) { dataView->showCommListFor(task); }

    void showTaskBox() { dataView->showTaskBox(); }

};

#endif
