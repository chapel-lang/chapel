/*
 * Copyright 2016-2020 Hewlett Packard Enterprise Development LP
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

    body = NULL;
}

SubView::~SubView()
{
  // printf ("SubView destructor for %s.", title->label());
  delete backBtn;
  delete title;
  if (body)  {
    remove(body);
    delete body;
  }
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

void CommListItemClickCB (Fl_Widget *w, void *p)
{
  ((SubView *)w->parent())->TaskCommCB();
}

bool SubView::ShowTaskComm (taskData *task)
{
  char tmpText[2048];
  const funcInfo* fInfo;
  const char * fName;

  // Save the task
  dispTask = task;
  
  // Time reference
  double startTime = VisData.start_clock();
  if (task->beginRec) 
    startTime = task->beginRec->clock_time();
  
  // Data lines
  SelectBrowser *theList = new SelectBrowser (x(), y()+24, w(), h()-24);
  theList->callback(CommListItemClickCB);
  std::list<Event *>::iterator itr;
  itr = task->commList.begin();
  while (itr != task->commList.end()) {
    E_fork *fp;
    E_comm *cp;
    switch ((*itr)->Ekind()) {
    case Ev_fork:
      fp = (E_fork *) *itr;
      fInfo = VisData.getFunctionInfo(fp->funcId());
      fName =  VisData.fileName(fInfo->fileNo);
      snprintf (tmpText, sizeof(tmpText),
                "[%f] %s to %d, argument size %d, function %s, file %s:%ld\n",
                fp->clock_time() - startTime,
                (fp->fast() ? "Fast On call" : "On call"),
                fp->dstId(), fp->argSize(), fInfo->name,
                (fName[0] == '$' ? &fName[11] : fName), fInfo->lineNo);
      theList->add(tmpText);
      break;
    case Ev_comm:
      cp = (E_comm *) *itr;
      fName = VisData.fileName(cp->srcFile());
      if (cp->isGet()) 
        snprintf (tmpText, sizeof(tmpText),
                  "[%f] Get from %d, total size %d, file %s:%ld\n",
                  cp->clock_time() - startTime, cp->srcId(), cp->totalLen(), 
                  (fName[0] == '$' ? &fName[11] : fName), cp->srcLine());
      else
        snprintf (tmpText, sizeof(tmpText),
                  "[%f] Put to %d, total size %d, file %s:%ld\n",
                  cp->clock_time() - startTime, cp->dstId(), cp->totalLen(),
                  (fName[0] == '$' ? &fName[11] : fName), cp->srcLine());
      theList->add(tmpText);
      break;
    default: // Do nothing
      break;
    }
    itr++;
  }
  theList->box(FL_NO_BOX);
  resizable(theList);
  body = theList;
  add(theList);
  return true;
}

void SubView::TaskCommCB (void)
{
  std::list<Event *>::iterator itr;
  SelectBrowser *theList = (SelectBrowser *)body;

  int ix = 1;  // Browser indexes start at 1, not 0
  itr = dispTask->commList.begin();

  while (itr != dispTask->commList.end()) {
    E_fork *fp;
    E_comm *cp;
    const funcInfo *fInfo;
    const char *fName;
    int lineNo;
    char text[512];

    switch ((*itr)->Ekind()) {
    case Ev_fork:
    case Ev_comm:
      if (theList->selected(ix)) {
        if ((*itr)->Ekind() == Ev_fork) {
          fp = (E_fork *) *itr;
          fInfo = VisData.getFunctionInfo(fp->funcId());
          fName =  VisData.fileName(fInfo->fileNo);
          lineNo = fInfo->lineNo;
          snprintf (text, sizeof(text),
                    "On call to %d at %s:%d", fp->dstId(),
                    (fName[0] == '$' ? &fName[11] : fName), lineNo);
        } else {
          cp = (E_comm *) *itr;
          fName = VisData.fileName(cp->srcFile());
          lineNo =  cp->srcLine();
          snprintf (text, sizeof(text),
                    "%s %d at %s:%d",
                    (cp->isGet() ? "Get from" : "Put to"),
                    (cp->isGet() ? cp->srcId() : cp->dstId()),
                    (fName[0] == '$' ? &fName[11] : fName), lineNo);
        }
        SubView *show  = new SubView (x(), y(), w(), h());
        show->headerText (text);
        show->showBackButton();
        if (!show->ShowFile (fName, lineNo)) {
          delete show;
          MainWindow->redraw();
          return;
        }
        DataField->pushNewChild (show, true);
      }
      ix++;
      break;
    default: // Do nothing
      break;
    }
    itr++;
  }
 
}
