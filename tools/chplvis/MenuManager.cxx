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

#include "chplvis.h"
#include <sstream>
#include <FL/Fl_File_Chooser.H>
#include <FL/fl_ask.H>

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
                    ( std::ostringstream() << std::dec << x ) ).str()

// Utility routine

bool open_data_set (const char *name, bool fromArgv)
{
  // printf("open_data_set ...\n");
  
  if (!VisData.LoadData(name,true)) {
    return false;
  }
  
  DataField->selectChild(GraphGrp);
  
  Menus.makeMenusFor(VIEW_GRAPH);
  Menus.setCurrentDataView(Graph_View);
  Menus.addPopUpTo(GraphGrp);
  Menus.setCurrentZoom(Graph_Scroll);
  Menus.setCurrentTag(DataModel::TagALL);
  
  Graph_View->setNumLocales(VisData.NumLocales());
  Graph_View->selectData(DataModel::TagALL);
  
  Grid_View->setNumLocales(VisData.NumLocales());
  Grid_View->selectData(DataModel::TagALL);
  
  Info->setFileName(name);
  Info->clearTagName();
  
  return true;
}

// Menu Callbacks

static void menu_cb_Open(Fl_Menu_*, void*) {
  char *selfile = fl_file_chooser("Open File", "*-0", "", 0);
  if (selfile) {
    // User didn't cancel
    if (!open_data_set(selfile, false)) {
      fl_alert ("Could not open %s", selfile);
    }
  };
}

static void menu_cb_Quit(Fl_Menu_*, void*) {
  if (VisSettings.save_WH) {
    VisSettings.saveToFile(false);
  }
  exit(0);
}

static void menu_cb_ZoomIn(Fl_Menu_*, void*) {
  if (Menus.currentZoom()) {
     Menus.currentZoom()->zoomIn();
     MainWindow->redraw();
  }
}

static void menu_cb_ZoomOut(Fl_Menu_*, void*) {
  if (Menus.currentZoom()) {
     Menus.currentZoom()->zoomOut();
     MainWindow->redraw();
  }
}

static void menu_cb_Reset(Fl_Menu_*, void*) {
  if (Menus.currentZoom()) {
     Menus.currentZoom()->reset();
     MainWindow->redraw();
  }
}

static void menu_cb_ShowTasks(Fl_Menu_*, void*) {
  Info->showTasks();
  MainWindow->redraw();
}

static void menu_cb_ShowCpu(Fl_Menu_*, void*) {
  Info->showCpu();
  MainWindow->redraw();
}

static void menu_cb_ShowClock(Fl_Menu_*, void*) {
  Info->showClock();
  MainWindow->redraw();
}

static void menu_cb_ShowConc(Fl_Menu_*, void*) {
  Info->showConcurrency();
  MainWindow->redraw();
}

static void menu_cb_ShowComm(Fl_Menu_*, void*) {
  Info->showComms();
  MainWindow->redraw();
}

static void menu_cb_ShowDsize(Fl_Menu_*, void*) {
  Info->showSize();
  MainWindow->redraw();
}

static void profile_cb_clock (Fl_Menu_*, void*)
{
  ProfileView->prepareData(ProfileBrowser::D_CLOCK);
}

static void profile_cb_tasks (Fl_Menu_*, void*)
{
  ProfileView->prepareData(ProfileBrowser::D_TASKS);
}

static void profile_cb_comm (Fl_Menu_*, void*)
{
  ProfileView->prepareData(ProfileBrowser::D_COMM);
}

static void profile_cb_oncall (Fl_Menu_*, void*)
{
  ProfileView->prepareData(ProfileBrowser::D_ONCALLS);
}

static void profile_cb_gets (Fl_Menu_*, void*)
{
  ProfileView->prepareData(ProfileBrowser::D_GETS);
}

static void profile_cb_puts (Fl_Menu_*, void*)
{
  ProfileView->prepareData(ProfileBrowser::D_PUTS);
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
  } else if (ix == DataModel::TagStart) {
    // printf ("cb_selTag called on Start\n");
    Info->setTagName("Start");
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
  }
  switch (Menus.currentViewKind()) {
  case VIEW_CONCURRENCY:
    concView->updateData (Menus.currentLoc(), ix);
    break;
  case VIEW_GRID:
  case VIEW_GRAPH:
    Menus.currentDataView()->selectData(ix);
    break;
  default:
    fprintf (stderr, "chplvis menu error!\n");
  }
  MainWindow->redraw();
}

static void cb_selectLocale(Fl_Widget *w, void *p)
{
  long ix = (long) p;
  Menus.setCurrentLoc(ix);
  concView->updateData (ix, Menus.currentTag());
  MainWindow->redraw();
}
  
static Fl_Menu_Item GridGraphMenu[] = {
 {"File", 0,  0, 0, FL_SUBMENU, FL_NORMAL_LABEL, 0, 14, 0},
 {"Open", 0x4006f,  (Fl_Callback*)menu_cb_Open, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Quit", 0x40071,  (Fl_Callback*)menu_cb_Quit, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"View", 0,  0, 0, FL_SUBMENU, FL_NORMAL_LABEL, 0, 14, 0},
 {"Zoom In", 0,  (Fl_Callback*)menu_cb_ZoomIn, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Zoom Out", 0,  (Fl_Callback*)menu_cb_ZoomOut, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Reset Zoom", 0,  (Fl_Callback*)menu_cb_Reset, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"Data", 0,  0, 0, FL_SUBMENU, FL_NORMAL_LABEL, 0, 14, 0},
 {"Locales", 0,  0, 0, FL_MENU_INACTIVE|FL_MENU_DIVIDER, FL_ENGRAVED_LABEL, 0, 14, 0},
 {"Show Tasks", 0,  (Fl_Callback*)menu_cb_ShowTasks, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show CPU", 0,  (Fl_Callback*)menu_cb_ShowCpu, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Clock", 0,  (Fl_Callback*)menu_cb_ShowClock, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Concurrency", 0,  (Fl_Callback*)menu_cb_ShowConc, 0, 128, FL_NORMAL_LABEL, 0, 14, 0},
 {"Communication", 0,  0, 0, FL_MENU_INACTIVE|FL_MENU_DIVIDER, FL_ENGRAVED_LABEL, 0, 14, 0},
 {"Show Count", 0,  (Fl_Callback*)menu_cb_ShowComm, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Size", 0,  (Fl_Callback*)menu_cb_ShowDsize, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0}
};

static Fl_Menu_Item gg_popup_Menu[] = {
 {"View", 0,  0, 0, FL_SUBMENU, FL_NORMAL_LABEL, 0, 14, 0},
 {"Zoom In", 0,  (Fl_Callback*)menu_cb_ZoomIn, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Zoom Out", 0,  (Fl_Callback*)menu_cb_ZoomOut, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Reset Zoom", 0,  (Fl_Callback*)menu_cb_Reset, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"Data", 0,  0, 0, FL_SUBMENU, FL_NORMAL_LABEL, 0, 14, 0},
 {"Locales", 0,  0, 0, FL_MENU_INACTIVE|FL_MENU_DIVIDER, FL_ENGRAVED_LABEL, 0, 14, 0},
 {"Show Tasks", 0,  (Fl_Callback*)menu_cb_ShowTasks, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show CPU", 0,  (Fl_Callback*)menu_cb_ShowCpu, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Clock", 0,  (Fl_Callback*)menu_cb_ShowClock, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Concurrency", 0,  (Fl_Callback*)menu_cb_ShowConc, 0, 128, FL_NORMAL_LABEL, 0, 14, 0},
 {"Communication", 0,  0, 0, FL_MENU_INACTIVE|FL_MENU_DIVIDER, FL_ENGRAVED_LABEL, 0, 14, 0},
 {"Show Count", 0,  (Fl_Callback*)menu_cb_ShowComm, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Size", 0,  (Fl_Callback*)menu_cb_ShowDsize, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0}
};

static Fl_Menu_Item ConcurrencyMenu[] = {
 {"File", 0,  0, 0, FL_SUBMENU, FL_NORMAL_LABEL, 0, 14, 0},
 {"Open", 0x4006f,  (Fl_Callback*)menu_cb_Open, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Quit", 0x40071,  (Fl_Callback*)menu_cb_Quit, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0}
};


static Fl_Menu_Item ProfileMenu[] = {
 {"File", 0, 0, 0, FL_SUBMENU, FL_NORMAL_LABEL, 0, 14, 0},
 {"Open", 0x4006f,  (Fl_Callback*)menu_cb_Open, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Quit", 0x40071,  (Fl_Callback*)menu_cb_Quit, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"Data", 0, 0, 0, FL_SUBMENU, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Clock", 0, (Fl_Callback*)profile_cb_clock, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Tasks", 0, (Fl_Callback*)profile_cb_tasks, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Communication", 0, (Fl_Callback*)profile_cb_comm, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show On Calls", 0, (Fl_Callback*)profile_cb_oncall, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Gets", 0, (Fl_Callback*)profile_cb_gets, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Puts", 0, (Fl_Callback*)profile_cb_puts, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0},
};

static Fl_Menu_Item profile_popupMenu[] = {
{"Data", 0, 0, 0, FL_SUBMENU, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Clock", 0, (Fl_Callback*)profile_cb_clock, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Tasks", 0, (Fl_Callback*)profile_cb_tasks, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Communication", 0, (Fl_Callback*)profile_cb_comm, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show On Calls", 0, (Fl_Callback*)profile_cb_oncall, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Gets", 0, (Fl_Callback*)profile_cb_gets, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Puts", 0, (Fl_Callback*)profile_cb_puts, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0},
};

static Fl_Menu_Item empty_popupMenu[] = {
 {0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0},
};
  


MenuManager::MenuManager()
{
  curView = VIEW_GRAPH;
  useUTags = false;
  curZoom = NULL;
  curDataView = NULL;
  curTag = DataModel::TagALL;
  popup = NULL;
}

void MenuManager::toggleUTags ()
{
  useUTags = !useUTags;
  makeTagsMenu();
  curTag = DataModel::TagALL;
  if (curDataView)
    curDataView->selectData(DataModel::TagALL);
}

void MenuManager::makeLocaleMenu (void)
{
  for (long ix = 0; ix < VisData.NumLocales(); ix++) {
    std::string entryName = "Locale/" + SSTR(ix);
    MainMenuBar->add(entryName.c_str(), 0, cb_selectLocale, (void *)ix);
    popup->add(entryName.c_str(), 0, cb_selectLocale, (void *)ix);
  }
}

void MenuManager::makeTagsMenu(void)
{
  // Remove the old one if it exists
  int tagMenu = MainMenuBar->find_index("Tags");
  int tagPopup; 

  if (tagMenu > 0) {
    MainMenuBar->remove(tagMenu);
    tagMenu = -1;
  }
  if (!popup) {
    popup = new Fl_Menu_Button (20,20,20,20);
    popup->type(Fl_Menu_Button::POPUP3);
    popup->box(FL_NO_BOX);
  }

  if (curView <= VIEW_GRID) 
    popup->menu(gg_popup_Menu);
  else if (curView == VIEW_PROFILE)
    popup->menu(profile_popupMenu);
  else
    popup->menu(empty_popupMenu);
  
  if (curView == VIEW_CONCURRENCY) {
    useUTags = false;
    makeLocaleMenu();
  }
  
  if (curView <= VIEW_CONCURRENCY && VisData.NumTags() >= 1) {
    
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
    if (curView != VIEW_CONCURRENCY && !VisData.hasUniqueTags()) {
      if (useUTags) {
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
    if (useUTags)
      numTags = VisData.NumUTags();
    else
      numTags = VisData.NumTags();
    // printf ("make menu: numTags = %ld\n", numTags);
    for (ix = 0; ix < numTags; ix++) {
      const char *tagName;
      std::string menuName;
      if (useUTags)
        tagName = VisData.getUTagData(ix)->name;
      else
        tagName = VisData.getTagData(ix)->name;
      if (VisData.hasUniqueTags() || useUTags)
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

void MenuManager::makeMenusFor(ViewKind viewkind)
{
  curView = viewkind;
  switch (viewkind) {
    case VIEW_GRAPH:
      MainMenuBar->menu(GridGraphMenu);
      curDataView = Graph_View;
      curZoom = Graph_Scroll;
      makeTagsMenu();
      addPopUpTo(DataField);
      Info->showTag();
      break;
      
    case VIEW_GRID:
      MainMenuBar->menu(GridGraphMenu);
      curDataView = Grid_View;
      curZoom = Grid_Scroll;
      makeTagsMenu();
      addPopUpTo(DataField);
      Info->showTag();
      break;

    case VIEW_CONCURRENCY:
      MainMenuBar->menu(ConcurrencyMenu);
      curDataView = NULL;
      curZoom = NULL;
      makeTagsMenu();
      addPopUpTo(DataField);
      Info->showTag();
      break;
      
    case VIEW_PROFILE:
      MainMenuBar->menu(ProfileMenu);
      curDataView = NULL;
      curZoom = NULL;
      makeTagsMenu();  // Actually removes the tags menu
      addPopUpTo(DataField);
      Info->hideTag();
      break;
  }
}
