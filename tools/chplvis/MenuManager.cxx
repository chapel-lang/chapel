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

#include "chplvis.h"
#include <sstream>

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
                    ( std::ostringstream() << std::dec << x ) ).str()


// Menu Callbacks

void menu_cb_ZoomIn(Fl_Menu_*, void*) {
  if (Menus.currentZoom())
     Menus.currentZoom()->zoomIn();
  MainWindow->redraw();
}

void menu_cb_ZoomOut(Fl_Menu_*, void*) {
  if (Menus.currentZoom())
     Menus.currentZoom()->zoomOut();
  MainWindow->redraw();
}

void menu_cb_Reset(Fl_Menu_*, void*) {
  if (Menus.currentZoom())
     Menus.currentZoom()->reset();
  MainWindow->redraw();
}

void menu_cb_ShowTasks(Fl_Menu_*, void*) {
  Info->showTasks();
  MainWindow->redraw();
}

void menu_cb_ShowCpu(Fl_Menu_*, void*) {
  Info->showCpu();
  MainWindow->redraw();
}

void menu_cb_ShowClock(Fl_Menu_*, void*) {
  Info->showClock();
  MainWindow->redraw();
}

void menu_cb_ShowConc(Fl_Menu_*, void*) {
  Info->showConcurrency();
  MainWindow->redraw();
}

void menu_cb_ShowComm(Fl_Menu_*, void*) {
  Info->showComms();
  MainWindow->redraw();
}

void menu_cb_ShowDsize(Fl_Menu_*, void*) {
  Info->showSize();
  MainWindow->redraw();
}

static void cb_toggleUnique(Fl_Widget *w, void*p) {
  Menus.toggleUTags();
  Menus.makeTagsMenu();
}

static void cb_selTag(Fl_Widget *w, void *p)
{
  long ix = (long) p;

  Menus.setCurrentTag(ix);
  if (ix == DataModel::TagALL) {
    // printf ("cb_selTag called on All\n");
    Info->setTagName("All");
    Menus.currentDataView()->selectData(ix);
  } else if (ix == DataModel::TagStart) {
    // printf ("cb_selTag called on Start\n");
    Info->setTagName("Start");
    Menus.currentDataView()->selectData(ix);
  } else {
    // printf ("cb_selTag called on tag %d \"%s\"\n", ptr->tagNo, ptr->tagName);
    if (Menus.usingUTags())
      Info->setTagName(VisData.getUTagData(ix)->name);
    else
      if (VisData.hasUniqueTags())
        Info->setTagName(VisData.getTagData(ix)->name);
      else {
        char tmp[2048];
        snprintf (tmp, sizeof(tmp), "%ld (%s)", ix, VisData.getTagData(ix)->name);
        Info->setTagName(tmp);
      }
    Menus.currentDataView()->selectData(ix);
  }
  MainWindow->redraw();
}

static Fl_Menu_Item popup_Menu[] = {
 {"View", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"Zoom In", 0,  (Fl_Callback*)menu_cb_ZoomIn, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Zoom Out", 0,  (Fl_Callback*)menu_cb_ZoomOut, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Reset Zoom", 0,  (Fl_Callback*)menu_cb_Reset, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"Data", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"Locales", 0,  0, 0, 129, FL_ENGRAVED_LABEL, 0, 14, 0},
 {"Show Tasks", 0,  (Fl_Callback*)menu_cb_ShowTasks, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show CPU", 0,  (Fl_Callback*)menu_cb_ShowCpu, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Clock", 0,  (Fl_Callback*)menu_cb_ShowClock, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Concurrency", 0,  (Fl_Callback*)menu_cb_ShowConc, 0, 128, FL_NORMAL_LABEL, 0, 14, 0},
 {"Communication", 0,  0, 0, 129, FL_ENGRAVED_LABEL, 0, 14, 0},
 {"Show Count", 0,  (Fl_Callback*)menu_cb_ShowComm, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Size", 0,  (Fl_Callback*)menu_cb_ShowDsize, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0}
};


MenuManager::MenuManager()
{
  useUTags = false;
  curZoom = NULL;
  curDataView = NULL;
  tagMenu = -1;
  tagPopup = -1;
  curTag = DataModel::TagALL;
}

void MenuManager::toggleUTags ()
{
  useUTags = !useUTags;
  makeTagsMenu();
  curTag = DataModel::TagALL;
  if (curDataView)
    curDataView->selectData(DataModel::TagALL);
}

void MenuManager::makeTagsMenu(void)
{
  // Remove the old one if it exists
  if (tagMenu > 0) {
    MainMenuBar->remove(tagMenu);
    tagMenu = -1;
  }
  if (!popup) {
    popup = new Fl_Menu_Button (20,20,20,20);
                                
    popup->type(Fl_Menu_Button::POPUP3);
    popup->box(FL_NO_BOX);
    popup->menu(popup_Menu);
  }
  if (tagPopup > 0) {
    popup->remove(tagPopup);
    tagPopup = -1;
  }
  if (VisData.NumTags() >= 1) {

    // printf("Make tags menu, %d tags\n", VisData.NumTags());

    // Build the menu

    tagMenu = MainMenuBar->add("Tags", 0, 0, 0, FL_SUBMENU);
    if (tagMenu < 1) {
      printf ("Menu problem!\n");
      return;
    }
    tagPopup = popup->add("Tags", 0, 0, 0, FL_SUBMENU);
    if (tagMenu < 1) {
      printf ("Popup menu problem!\n");
      return;
    }
    if (!VisData.hasUniqueTags()) {
      if (Menus.usingUTags()) {
        MainMenuBar->add("Tags/Show All Tags", 0, cb_toggleUnique, (void *)0);
        popup->add("Tags/Show All Tags", 0, cb_toggleUnique, (void *)0);
      }
      else {
        MainMenuBar->add("Tags/Merge Tags", 0, cb_toggleUnique, (void *)0);
        popup->add("Tags/Merge Tags", 0, cb_toggleUnique, (void *)0);
      }
    }
    MainMenuBar->add("Tags/All", 0, cb_selTag, (void *)DataModel::TagALL);
    popup->add("Tags/All", 0, cb_selTag, (void *)DataModel::TagALL);
    MainMenuBar->add("Tags/Start", 0, cb_selTag, (void *)DataModel::TagStart);
    popup->add("Tags/Start", 0, cb_selTag, (void *)DataModel::TagStart);

    long ix;
    long numTags;
    if (Menus.usingUTags())
      numTags = VisData.NumUTags();
    else
      numTags = VisData.NumTags();
    // printf ("make menu: numTags = %ld\n", numTags);
    for (ix = 0; ix < numTags; ix++) {
      const char *tagName;
      std::string menuName;
      if (Menus.usingUTags())
        tagName = VisData.getUTagData(ix)->name;
      else
        tagName = VisData.getTagData(ix)->name;
      if (VisData.hasUniqueTags() || Menus.usingUTags())
        menuName = "Tags/" + std::string(tagName);
      else
        menuName = "Tags/tag " + SSTR(ix) + " (" + tagName + ")";
      MainMenuBar->add(menuName.c_str(), 0, cb_selTag, (void *)ix, 0);
      popup->add(menuName.c_str(), 0, cb_selTag, (void *)ix, 0);
    }
    MainMenuBar->redraw();
  }
}

void MenuManager::addPopUpTo(Fl_Group *group)
{
  if (popup) {
    group->add(popup);
    popup->resize(group->x(), group->y(), group->w(), group->h());
  }
}

void MenuManager::setCurrentDataView(DataView *view) { curDataView = view; }
