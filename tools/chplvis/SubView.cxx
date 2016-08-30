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


#include "SubView.h"
#include <FL/Fl_Browser.H>
#include <FL/fl_ask.H>
#include <errno.h>
#include "chplvis.h"
#include "LeftChevron-20x20.xpm"

void stdBackCallback (Fl_Widget *w, void *p)
{
  DataField->popChild();
}


SubView::SubView (int x, int y, int w, int h, const char *l)
  : Fl_Group(x, y, w, h, l)
{
    Fl_Pixmap *pm = new Fl_Pixmap(left_chevron20x20);
    backBtn = new Fl_Button (x+2,y+2,20,20,0);
    backBtn->box(FL_NO_BOX);
    backBtn->image(pm);
    backBtn->callback(stdBackCallback, (void *)0);
    backBtn->clear_visible_focus();
    add(backBtn);
    backBtn->hide();

    title = new Fl_Button (x+24, y, w-24, 24);
    title->box(FL_NO_BOX);
    title->clear_visible_focus();
    add(title);
}

SubView::~SubView()
{
  printf ("SubView destructor for %s.", title->label());
  delete backBtn;
  delete title;
  if (body) delete body;
}


void SubView::showBackButton (void (*cb)(Fl_Widget *))
{
  backBtn->callback(cb);
  backBtn->show();
}

void SubView::showBackButton (void)
{
  backBtn->show();
}

void SubView::headerText (const char *t)
{
  title->copy_label(t);
}


// Specifics for each kind ...

bool SubView::ShowFile (const char *filename, int lineNo)
{
  char FinalName[1024];
  // Check for known 'bad' file names
  if (filename[0] == '<' && 
      (strcmp(filename, "<unknown>") == 0
       || strcmp(filename, "<internal>") == 0)) {
    fl_alert("No file to open.");
  }

  // Make correct file name.
  if (filename[0] == '$') {
    const char *home = VisData.CHPL_HOME();
    //printf ("Opening file: '%s/%s'\n",  home, &filename[11]);
    snprintf (FinalName, sizeof(FinalName), "%s/%s", home, &filename[11]);
  } else if (filename[0] != '/') {
    const char *dir = VisData.DIR();
    snprintf (FinalName, sizeof(FinalName), "%s/%s", dir, filename);
  } else {
    strncpy (FinalName, filename, sizeof(FinalName));
  }
  
  Fl_Browser *fb = new Fl_Browser (x(), y()+24, w(), h()-24);
  if (!fb->load(FinalName)) {
    fl_alert("Could not open %s: %s", FinalName, strerror(errno));
    return false;
  }
  fb->select(lineNo);
  fb->box(FL_NO_BOX);
  resizable(fb);
  body = fb;
  add(fb);
  return true;
}
