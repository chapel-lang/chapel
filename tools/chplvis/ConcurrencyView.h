/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2016-2019 Cray Inc.
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

#ifndef CONCURRENCYVIEW_H
#define CONCURRENCYVIEW_H

#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Button.H>

#include "DataModel.h"

class ConcurrencyView;

// Data (Box) class for the concurrency view

struct drawData {
  int startLine;
  int column;
  int endLine;
  bool isEndTask;
};

class ConcurrencyData : public Fl_Group {

  private:
    ConcurrencyView *parent;
    std::list<drawData> drawDB;

  public:
    void draw (void);
    int handle (int event);

    ConcurrencyData (int x, int y, int W, int H, const char *l=0) : Fl_Group(x,y,W,H,l){};

    void setParent(ConcurrencyView *p) { parent = p; }

    void buildData (void);
  
};

//  View for showing concurrency details.

class ConcurrencyView : public Fl_Group {

 friend class ConcurrencyData;

  private:
    Fl_Box *title;
    Fl_Scroll *scroll;
    ConcurrencyData *dataBox;
    Fl_Text_Display *commBox;
    Fl_Button *backBtn;

    long localeNum;
    long tagNum;

    DataModel::tagData *curTag;  

  public:
    ConcurrencyView (int x, int y, int W, int H, const char *l=0);

    void setMembers (Fl_Box *ttl, Fl_Scroll *scrl, ConcurrencyData *data) {
      title = ttl;
      scroll = scrl;
      dataBox = data;
      curTag = NULL;
      tagNum = -3;
      localeNum = 0;
      dataBox->setParent(this);
      commBox = NULL;
    }

    void updateData (long loc, long tag);

    void showCommListFor(taskData *task);

    void showTaskBox();

};

#endif
