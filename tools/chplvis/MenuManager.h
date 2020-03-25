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

#ifndef MENUMANAGER_H
#define MENUMANAGER_H

// Multiple places need to modify the menus, both
// main menu bar and popup menus.  This puts the
// code in a common place.


#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Group.H>
#include "DataView.h"
#include "ZoomScroll.h"

enum ViewKind {VIEW_GRAPH, VIEW_GRID, VIEW_CONCURRENCY, VIEW_PROFILE};

class MenuManager {

  bool useUTags;
  ViewKind curView;

  DataView *curDataView;
  ZoomScroll *curZoom;
  
  Fl_Menu_Button *popup;

  long curTag;
  long lastLoc;
    
 public:
  MenuManager();

  bool usingUTags() { return useUTags; }

  void toggleUTags();

  ViewKind currentViewKind() { return curView; }
  
  DataView *currentDataView() { return curDataView; }

  void setCurrentDataView(DataView *view);
  
  ZoomScroll *currentZoom() {return curZoom;}

  void setCurrentZoom(ZoomScroll *view) { curZoom = view; }

  void makeLocaleMenu (void);

  void makeTagsMenu (void);

  void addPopUpTo(Fl_Group *group);

  long currentTag() { return curTag; }
  void setCurrentTag(long tag) { curTag = tag; }

  long currentLoc() { return lastLoc; }
  void setCurrentLoc(long l) { lastLoc = l; }

  void makeMenusFor(ViewKind v);
  
};


#endif
