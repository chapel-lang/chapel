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

#ifndef DATAVIEW_H
#define DATAVIEW_H

//class DataView;  // So we can include chplvis.h

#include <FL/Fl.H>
#include <FL/Fl_Box.H>

#include "DataModel.h"

#include <string>

// DataView is the base class for viewing data in the
// chplvis program.   It is extened by each class that
// presents data to the user.
//
//    Graph View -- Shows locales as vertices and communication as arcs
//    Grid View -- Locales are shown in a grid with no edges.


class DataView : public Fl_Box {

  protected:

    int numlocales;

    DataModel::tagData *curTagData;
    int curTagNum;

  public:

 DataView(int x, int y, int w, int h, const char *l=0) 
   : Fl_Box(x, y, w, h, l) 
      {
        numlocales = 1;
        curTagData = 0;
        curTagNum = 0;
      };

  //  Virtual methods to override
  void draw (void) { Fl_Box::draw(); }
  int handle (int event) { return Fl_Box::handle(event); }

  // Processing routines and virtual methods

  virtual void selectData (int tagNum) = 0;

  virtual void setNumLocales (int n) = 0;

  virtual int  getNumLocales (void) { return numlocales; }
  
};

#endif
