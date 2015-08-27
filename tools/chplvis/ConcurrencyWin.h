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

#ifndef CONCURRENCYWIN_H
#define CONCURRENCYWIN_H

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Scroll.H>

#include "DataModel.h"

class ConcurrencyWin;

// Data (Box) class for the concurrency window

struct drawData {
  int startLine;
  int column;
  int endLine;
  bool isEndTask;
};

class ConcurrencyData : public Fl_Group {

  private:
    ConcurrencyWin *parent;
    std::list<drawData> drawDB;

  public:
    void draw (void);
    int handle (int event);

    ConcurrencyData (int x, int y, int W, int H, const char *l=0) : Fl_Group(x,y,W,H,l){};

    void setParent(ConcurrencyWin *p) { parent = p; }

    void buildData (void);
  
};

// Window for showing concurrency details.

class ConcurrencyWin : public Fl_Double_Window {

 friend class ConcurrencyData;

  private:
    Fl_Box *title;
    Fl_Scroll *scroll;
    ConcurrencyData *dataBox;

    long localeNum;
    long tagNum;

    DataModel::tagData *curTag;  

  public:
    ConcurrencyWin (int x, int y, int W, int H, const char *l=0);

    void setMembers (Fl_Box *ttl, Fl_Scroll *scrl, ConcurrencyData *data) {
      title = ttl;
      scroll = scrl;
      dataBox = data;
      curTag = NULL;
      tagNum = -3;
      localeNum = 0;
      dataBox->setParent(this);
    }

    void updateData (long loc, long tag);

};

#endif
