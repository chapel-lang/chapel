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

// This class is the primary view field for chplvis.   Most of the visualization
// is done by special drawing done by this class


#include "chplvis.h"
#include "math.h"
#include <FL/fl_draw.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Menu_Item.H>
#include <sstream>

// Menu Callbacks

static void cb_ZoomIn(Fl_Menu_*, void*) {
  GridScroll->zoomIn();
}

static void cb_ZoomOut(Fl_Menu_*, void*) {
  GridScroll->zoomOut();
}

static void cb_Reset(Fl_Menu_*, void*) {
  GridScroll->reset();
}

static void cb_ShowTasks(Fl_Menu_*, void*) {
  GrdView->showTasks();
  MainWindow->redraw();
}

static void cb_ShowCpu(Fl_Menu_*, void*) {
  GrdView->showCpu();
  MainWindow->redraw();
}

static void cb_ShowClock(Fl_Menu_*, void*) {
  GrdView->showClock();
  MainWindow->redraw();
}

static void cb_ShowConc(Fl_Menu_*, void*) {
  GrdView->showConcurrency();
  MainWindow->redraw();
}

static void cb_ShowComm(Fl_Menu_*, void*) {
  GrdView->showComms();
  MainWindow->redraw();
}

static void cb_ShowDsize(Fl_Menu_*, void*) {
  GrdView->showDsize();
  MainWindow->redraw();
}


static Fl_Menu_Item popup_Menu[] = {
 {"View", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"Zoom In", 0,  (Fl_Callback*)cb_ZoomIn, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Zoom Out", 0,  (Fl_Callback*)cb_ZoomOut, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Reset Zoom", 0,  (Fl_Callback*)cb_Reset, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"Data", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"Locales", 0,  0, 0, 129, FL_ENGRAVED_LABEL, 0, 14, 0},
 {"Show Tasks", 0,  (Fl_Callback*)cb_ShowTasks, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show CPU", 0,  (Fl_Callback*)cb_ShowCpu, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Clock", 0,  (Fl_Callback*)cb_ShowClock, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Concurrency", 0,  (Fl_Callback*)cb_ShowConc, 0, 128, FL_NORMAL_LABEL, 0, 14, 0},
 {"Communication", 0,  0, 0, 129, FL_ENGRAVED_LABEL, 0, 14, 0},
 {"Show Count", 0,  (Fl_Callback*)cb_ShowComm, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Show Size", 0,  (Fl_Callback*)cb_ShowDsize, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0}
};


#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
                    ( std::ostringstream() << std::dec << x ) ).str()

//  GridView Constructors

GridView::GridView (int bx, int by, int bw, int bh, const char *label)
  : DataView (bx, by, bw, bh, 0) 
{
  // printf ("GridView init. h=%d, w=%d, numlocales is %d\n",bh,bw, VisData.NumLocales());
  numlocales = 0;
  theLocales = NULL;
  curTagData = NULL;
  tagMenu = -1;
  tagPopup = -1;
  popup = NULL;  
  useUTags = false;
  if (VisData.NumLocales() > 0) {
    setNumLocales(VisData.NumLocales());
  }
  infoTop = show_Tasks;
};

// Private methods 

void GridView::allocArrays()
{
  int ix;

  printf ("allocArrays\n");
  // Dealloc anything current
  if (theLocales != NULL) {
    for (ix = 0; ix < getSize; ix++) {
      if (theLocales[ix].b != NULL) 
        delete theLocales[ix].b;
    }
    delete [] theLocales;
  }
  
  // Alloc new space
  theLocales = new localeInfo [numlocales];
  if (theLocales == NULL) {
    fprintf (stderr, "chplvis: out of memory.\n");
    exit(1);
  }
  getSize = numlocales;

  for (ix = 0; ix < numlocales; ix++) {
    theLocales[ix].b = NULL;
  }
}


// Public Methods 

//  tagNo:  -3 => new open
//          -2 => All: start to finish
//          -1 => Start to first tag (tag 0)
//          0 and greater => start at tagNo to next tag.

void GridView::selectData(int tagNum)
{

  // Select data to use
  if (useUTags)
    curTagData = VisData.getUTagData(tagNum);
  else
    curTagData = VisData.getTagData(tagNum);
  curTagNum = tagNum;

  // Set the max values in the info bar
  Info->setMaxes(curTagData->maxTasks, curTagData->maxComms, curTagData->maxSize,
                 curTagData->maxConc, curTagData->maxCpu, curTagData->maxClock);

 }

static void selTag(Fl_Widget *w, void *p)
{
  long ix = (long) p;
  
  if (ix == DataModel::TagALL) {
    // printf ("selTag called on All\n");
    Info->setTagName("All");
    GrdView->selectData(ix);
  } else if (ix == DataModel::TagStart) {
    // printf ("selTag called on Start\n");
    Info->setTagName("Start");
    GrdView->selectData(ix);
  } else {
    // printf ("selTag called on tag %d \"%s\"\n", ptr->tagNo, ptr->tagName);
    if (GrdView->usingUTags())
      Info->setTagName(VisData.getUTagData(ix)->name);
    else
      if (VisData.hasUniqueTags())
        Info->setTagName(VisData.getTagData(ix)->name);
      else {
        char tmp[2048];
        snprintf (tmp, sizeof(tmp), "%ld (%s)", ix, VisData.getTagData(ix)->name);
        Info->setTagName(tmp);
      }
    GrdView->selectData(ix);
  }
  MainWindow->redraw();
}

static void toggleUnique (Fl_Widget *w, void *p) {
  GrdView->toggleUTags();
  GrdView->makeTagsMenu();
  selTag((Fl_Widget *)NULL, (void *)DataModel::TagALL);
}

void GridView::makeTagsMenu(void)
{
  // Remove the old one if it exists
  if (tagMenu > 0) {
    MainMenuBar->remove(tagMenu);
    tagMenu = -1;
  }
  if (!popup) {
    popup = new Fl_Menu_Button (GraphGrp->x(), GraphGrp->y(),
                                GraphGrp->w(), GraphGrp->h());
    popup->type(Fl_Menu_Button::POPUP3);
    popup->box(FL_NO_BOX);
    popup->menu(popup_Menu);
    GraphGrp->add(popup);
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

    tagMenu = popup->add("Tags", 0, 0, 0, FL_SUBMENU);
    if (tagMenu < 1) {
      printf ("Popup menu problem!\n");
      return;
    }

    if (!VisData.hasUniqueTags()) {
      if (useUTags) {
        MainMenuBar->add("Tags/Show All Tags", 0, toggleUnique, (void *)0);
        popup->add("Tags/Show All Tags", 0, toggleUnique, (void *)0);
      }
      else {
        MainMenuBar->add("Tags/Merge Tags", 0, toggleUnique, (void *)0);
        popup->add("Tags/Merge Tags", 0, toggleUnique, (void *)0);
      }
    }
    MainMenuBar->add("Tags/All", 0, selTag, (void *)DataModel::TagALL);
    popup->add("Tags/All", 0, selTag, (void *)DataModel::TagALL);
    MainMenuBar->add("Tags/Start", 0, selTag, (void *)DataModel::TagStart);
    popup->add("Tags/Start", 0, selTag, (void *)DataModel::TagStart);

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
      MainMenuBar->add(menuName.c_str(), 0, selTag, (void *)ix, 0);
      popup->add(menuName.c_str(), 0, selTag, (void *)ix, 0);
    }
    MainMenuBar->redraw();
  }
}

void GridView::setTooltip ( int ix, bool isInt, int ival, double fval)
{
    char tmpchars[100];
    if (theLocales != NULL) {
      localeInfo *loc = &theLocales[ix];

      // Sets up an invisible box under each locale that responds to tooltip requests!
      loc->x = x() + (ix % ncols)*40;
      loc->y = y() + (ix / nrows)*40;
      loc->w = 30;
      loc->h = 30;

      // Invisible button for the tooltip!
      if (loc->b == NULL) {
        loc->b = new Fl_Box (FL_NO_BOX, loc->x-loc->w/2, loc->y-loc->h/2, loc->w, loc->h, NULL);
        parent()->add(loc->b);
        loc->b->show();
      } else {
        loc->b->position(loc->x-loc->w/2,loc->y-loc->h/2);
        loc->b->size(loc->w,loc->h);
      }
      if (isInt)
        snprintf (tmpchars, sizeof(tmpchars), "%d->%d", ix, ival);
      else
        snprintf (tmpchars, sizeof(tmpchars), "%d->%lf", ix, fval);
      loc->b->copy_tooltip(tmpchars);
    }
}

void GridView::drawLocale ( int ix, Fl_Color col)
{
  printf ("drawLocale %d\n", ix);
  char ixchars[10];
  if (theLocales != NULL) {
    localeInfo *loc = &theLocales[ix];
    
    // Draw a locale ...
    loc->x = x() + (ix % ncols)*40;
    loc->y = y() + (ix / nrows)*40;
    snprintf (ixchars, 10, "%d", ix);
    loc->w = 30;
    loc->h = 30;
    
    fl_color(col);
    fl_rectf(loc->x-loc->w/2, loc->y-loc->h/2, loc->w, loc->h);
    fl_color(FL_BLACK);
    fl_rect(loc->x-loc->w/2, loc->y-loc->h/2, loc->w, loc->h);
    fl_draw(ixchars, loc->x+3-loc->w/2, loc->y+3-loc->h/2,
            loc->w-6, loc->h-6, FL_ALIGN_CENTER, NULL, 0);
  }
}

void GridView::draw()
{
  int ix;

  // printf ("GridView draw, numlocales is %d\n", numlocales);
  // printf ("draw: x,y = %d,%d, h,w = %d,%d\n", x(), y(), w(), h());

  for (ix = 0; ix < numlocales; ix++) {
    switch (infoTop) {
    case show_Tasks:
      setTooltip(ix, true, curTagData->locales[ix].numTasks, 0);
      break;
    case show_CPU:
      setTooltip(ix, false, 0, curTagData->locales[ix].Cpu);
      break;
    case show_Clock:
      setTooltip(ix, false, 0, curTagData->locales[ix].clockTime);
      break;
    case show_Concurrency:
      setTooltip(ix, true, curTagData->locales[ix].maxConc, 0);
    }
  }

  DataView::draw();

  // Draw locales next
  
  for (ix = 0; ix < numlocales; ix++) {
    printf ("Draw Locale %d.\n", ix);
    switch (infoTop) {
    case show_Tasks:
      //drawLocale(ix,FL_RED);
      drawLocale(ix, heatColor(curTagData->locales[ix].numTasks, curTagData->maxTasks));
      break;
    case show_CPU:
      drawLocale(ix, heatColor(curTagData->locales[ix].Cpu, curTagData->maxCpu));
      break;
    case show_Clock:
      drawLocale(ix, heatColor(curTagData->locales[ix].clockTime, curTagData->maxClock));
      break;
    case show_Concurrency:
      drawLocale(ix, heatColor(curTagData->locales[ix].maxConc, curTagData->maxConc));
    }
  }

  // Set up the info widget correctly
  if (showcomms)
    Info->showComms();
  else 
    Info->showSize();
  switch (infoTop) {
    case show_Tasks:
      Info->showTasks();
      break;
    case show_CPU:
      Info->showCpu();
      break;
    case show_Clock:
      Info->showClock();
      break;
    case show_Concurrency:
      Info->showConcurrency();
      break;
  }
    
  Info->draw();
}


int GridView::handle(int event)
{
  int ix;

  int x = Fl::event_x();
  int y = Fl::event_y();

  int i, j; // search for the comm link clicked
  
  switch (event) {
  case FL_PUSH:
    //printf ("Push at (%d,%d) event_button() = %d\n", x, y, Fl::event_buttons());
    //if (Fl::event_buttons() == FL_RIGHT_MOUSE) {
    //  printf ("right mouse push\n");
    //}
    break;
  case FL_RELEASE:
    //printf ("Release at (%d,%d)\n", x, y);
    if (Fl::event_button() == FL_MIDDLE_MOUSE ||
        Fl::event_button() == FL_RIGHT_MOUSE) {
      // printf ("middle or right release\n");
      break;
    }
#if 0    
    // Click on a locale?
    if (numlocales > 0) {
      for (ix = 0; ix < numlocales; ix++) {
        // See if release is inside a locale
        localeInfo *loc = &theLocales[ix];
        if ( x > loc->x-loc->w/2 && x <= loc->x + loc->w/2 &&
             y > loc->y-loc->h/2 && y <= loc->y + loc->h/2) {
          if (infoTop == show_Concurrency) {
            if (useUTags && curTagNum != DataModel::TagALL) {
              fl_alert("Concurrency view available only for tag 'ALL' in merged tag mode.");
            } else {
              if (theLocales[ix].ccwin == NULL) {
                // Create the window
                theLocales[ix].ccwin = make_concurrency_window(ix, curTagNum);
              } else {
                theLocales[ix].ccwin->updateData(ix, curTagNum);
                theLocales[ix].ccwin->showTaskBox();
              }
              if (theLocales[ix].ccwin->visible())
                theLocales[ix].ccwin->hide();
              else
                theLocales[ix].ccwin->show();
            }
          } else {
            if (theLocales[ix].win == NULL) {
              // Create the window
              theLocales[ix].win = make_locale_window(ix, &curTagData->locales[ix]);
            } else {
              theLocales[ix].win->updateWin(&curTagData->locales[ix]);
            }
            if (theLocales[ix].win->visible()) 
              theLocales[ix].win->hide();
            else
              theLocales[ix].win->show();
          }
          return 1;
        }
      }
    }
    break;
#endif    
  }
  return DataView::handle(event);
}

void GridView::redrawAllWindows(void)
    {
      int ix, ix1, ix2;
      MainWindow->redraw();
    }
