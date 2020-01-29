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

#include <stdio.h>
#include <assert.h>

#include "ConcurrencyView.h"
#include "chplvis.h"

#include "SubView.h"
#include "SelectBrowser.h"

#include <FL/fl_draw.H>
#include <FL/fl_ask.H>

// Callback for this view

void backCallback (Fl_Widget *w, void *p) {
  ConcurrencyView *view = (ConcurrencyView *)w->parent();
  view->showTaskBox();
  //printf ("backButtonClicked\n");
  //printf ("w is 0x%lx\n", (long)w); 
  //printf ("v is 0x%lx\n", (long)view); 
}

void taskCallback (Fl_Widget *w, void *p) {
  ConcurrencyView *view = (ConcurrencyView *)w->parent()->parent()->parent();
  view->showCommListFor((taskData *)p);
  //printf ("task box clicked.\n");
  //printf ("w is 0x%lx\n", (long)w); 
  //printf ("v is 0x%lx\n", (long)view); 
}

ConcurrencyView::ConcurrencyView (int x, int y, int W, int H, const char *l)
  :  Fl_Group (x, y, W, H, l)
{
  //printf ("Concurrency View is 0x%lx\n", (long)this);
}

void ConcurrencyView::updateData (long loc, long tag)
{
  char tmp[2084];
  localeNum = loc;
  tagNum = tag;
  curTag = VisData.getTagData(tagNum);
  snprintf(tmp, sizeof(tmp), "Concurrency for Locale %ld, tag %s, max %ld, max Clock %f",
           loc, curTag->name, curTag->locales[loc].maxConc,
           curTag->locales[loc].maxTaskClock);
  title->copy_label(tmp);
  dataBox->buildData();
}


void ConcurrencyView::showCommListFor(taskData *task)
{
  if (task->commSum.numComms == 0) {
    fl_alert("Task has no communications to show.");
  } else {

    SubView *commList = new SubView(x(),y(),w(),h());
    commList->showBackButton();
    
   // Title
    char tmpText[2048];
    if (task->taskRec) 
      snprintf (tmpText, sizeof(tmpText), "Communications for task %ld on locale %d.",
                task->taskRec->taskId(), task->taskRec->nodeId());
    else // Main should be the only one without a taskRec
      snprintf (tmpText, sizeof(tmpText), "Communications for main task on locale 0.");
    commList->headerText(tmpText);

    if (!commList->ShowTaskComm(task)) {
      fl_alert("Internal error?");
      delete commList;
    }
     
    DataField->pushNewChild(commList, true);
  }
}

void ConcurrencyView::showTaskBox() {
  if (commBox != NULL) {
    remove(commBox);
    delete commBox;
    commBox = NULL;
    remove(backBtn);
    delete(backBtn);
    backBtn = NULL;
    scroll->show();
    resizable(scroll);
    redraw();
  }
}


// Concurrency Data 
void ConcurrencyData::draw (void) {

  int s_x, s_y, s_w, s_h, s_xpos, s_ypos;  // Scroll information

  int l_x;   // Line x placement
  int l_y1;  // Line y start
  int l_y2;  // Line y stop
  bool y2_clip;  // Did y2 get clipped

  Fl_Group::draw();
  std::list<drawData>::iterator dbItr;

  // Get the scroll window information for "clipping" and drawing
  s_x = parent->scroll->x();
  s_y = parent->scroll->y();
  s_xpos = parent->scroll->xposition();
  s_ypos = parent->scroll->yposition();
  s_w = parent->scroll->w();
  s_h = parent->scroll->h();

  // printf ("drawing %d,%d -- %d,%d\n", s_xpos, s_ypos, s_w, s_h);

  for (dbItr = drawDB.begin(); dbItr != drawDB.end(); dbItr++) {
    l_x = 45 + 60 * dbItr->column;
    l_y1 = 30 + 25 * dbItr->startLine;
    l_y2 = 30 + 25 * dbItr->endLine + (dbItr->isEndTask ? 0 : 5);
    y2_clip = false;

    if (l_x < s_xpos-1 || l_x > s_xpos + s_w + 1 // Horizontally out
        || l_y2 < s_ypos || l_y1 > s_ypos + s_h )
      continue;  // Out of scroll area, don't draw lines

    // Translate to window coords
    l_x = l_x - s_xpos;
    l_y1 = l_y1-s_ypos; 
    l_y2 = l_y2-s_ypos;
    if (l_y2 > s_h) {
      l_y2 = s_h;
      y2_clip = true;
    }

    // Draw vertical line
    fl_color(FL_BLACK);
    fl_line_style(FL_SOLID, 3, NULL);
    fl_line(s_x + l_x, s_y + l_y1+30, s_x + l_x, s_y + l_y2+30);

    if (dbItr->isEndTask && !y2_clip) {
      // Draw the end of task line
      fl_line(s_x + l_x-5, s_y + l_y2+30, s_x + l_x+5, s_y + l_y2+30);
    }
  }
  fl_line_style(FL_SOLID, 1, NULL);
}

int ConcurrencyData::handle(int event) {
  return Fl_Group::handle(event);
}


void ConcurrencyData::buildData(void) {
  Fl_Box *b;
  Fl_Button *btn;
  char tmp[2048];
  int width = w();
  int height = h();
  int progMaxConc =  VisData.getTagData(DataModel::TagALL)->maxConc;
  int greedy[progMaxConc+1];
  int greedyStart[progMaxConc];
  int numLines = 0;
  int curLine;
  int curCol;

  DataModel::tagData *tmpTag;
  long tmpTagNo;

  std::list <DataModel::timelineEntry>::iterator tl_itr;
  std::list <DataModel::timelineEntry>::iterator tagStart;

  taskData *theTask;
  bool done;

  drawData lineDrawData;

  // Get rid of all children
  clear();
  drawDB.clear();

  // initialize
  for (int ix = 0; ix < progMaxConc; ix++)  {
    greedy[ix] = 0;
    greedyStart[ix] = 0;
  }
  greedy[progMaxConc] = 0;  // Sentinal

  // Special case for locale 0 .. main thread of 1
  if (parent->localeNum == 0)
    greedy[0] = 1;

  // Calculate new height and width
  // Default width ... unless we get a more specific one.
  width = 30 + 60 * VisData.getTagData(DataModel::TagALL)->
              locales[parent->localeNum].maxConc;
  if (parent->tagNum == DataModel::TagALL) {
    numLines =  VisData.taskTimeline[parent->localeNum].size();
    tagStart = VisData.taskTimeline[parent->localeNum].begin();
  } else {
    // Walk the timeline keeping tasks that go past the tag
    tl_itr = VisData.taskTimeline[parent->localeNum].begin();
    tmpTag = VisData.getTagData(DataModel::TagStart);
    tmpTagNo = DataModel::TagStart;
    if (parent->tagNum == DataModel::TagStart)
      tagStart = tl_itr;

    // counting the number of lines if in the tag
    while (tl_itr !=  VisData.taskTimeline[parent->localeNum].end()) {
      if (tl_itr->first != DataModel::Tl_Tag && tmpTagNo == parent->tagNum) 
        numLines++;
      switch (tl_itr->first) {

        case DataModel::Tl_Tag:
          tmpTagNo = tl_itr->second;
          tmpTag = VisData.getTagData(tmpTagNo);
          if (tmpTagNo == parent->tagNum) {
            tagStart = tl_itr;
            tagStart++; // Move to the first record after the tag
            width = 30 + 60 * tmpTag->locales[parent->localeNum].maxConc;
          }
          break;

        case DataModel::Tl_Begin:
          if (tmpTagNo < parent->tagNum) {
            theTask =  VisData.getTaskData(parent->localeNum, tl_itr->second, tmpTagNo);
            if (theTask && theTask->taskRec != NULL && theTask->endTagNo != tmpTagNo) {
              // printf ("should be greedy: tag %ld\n", tl_itr->second);
              for (int i = 0; i < progMaxConc; i++) {
                if (greedy[i] == 0) {
                  greedy[i] = tl_itr->second;
                  greedyStart[i] = tmpTagNo;
                  break;
                }
                // If this is true, progMaxConc is wrong!
                assert (i+1 != progMaxConc);
              }
            } else {
              if (theTask && theTask->taskRec == NULL) 
                printf ("Didn't find task %ld/%ld in tag %ld\n", parent->localeNum,
                        tl_itr->second, tmpTagNo);
            }
          }
          break;

        default:
          break;
      }
      if (tmpTagNo > parent->tagNum)
        break;
      tl_itr++;
    }
  }

  // Get correct number of lines and height of data area
  if (greedy[0]) numLines++;
  height = 40 + 25 * numLines;
                                   
  // Reset scroll
  parent->scroll->scroll_to(0,0);

  // Don't make the size too small.
  if (width < 450)
    width = 450;
  if (height < 450)
    height = 450;

  // printf ("resize data size:  %dx%d\n", width, height);

  // Start the rebuild by resizing
  resize (x(), y(), width, height);

  // Build the data
  curLine = 0;
  tl_itr = tagStart;
  tmpTagNo = parent->tagNum;
  if (tmpTagNo == DataModel::TagALL)
    tmpTagNo = DataModel::TagStart;
  //tmpTag = VisData.getTagData(tmpTagNo);

  // Build the continuition
  curCol = 0;
  for (int col = 0; col < progMaxConc; col++)
    if (greedy[col] != 0) {
      // printf ("Building continuation for col %d\n", col);
      greedy[curCol] = greedy[col];
      greedyStart[curCol] = 0;
      btn = new Fl_Button(x()+15+60*curCol, y()+40, 60, 20, NULL);
      btn->box(FL_BORDER_BOX);
      btn->down_box(FL_BORDER_BOX);
      if (parent->localeNum == 0 && greedy[col] == 1)
        snprintf (tmp, sizeof(tmp), "Main");
      else
        snprintf (tmp, sizeof(tmp), "C %d", greedy[curCol]);
      btn->copy_label(tmp);
      add(btn);
      theTask = VisData.getTaskData(parent->localeNum, greedy[curCol]);
      if (theTask) {
        if (theTask->taskRec && theTask->taskRec->isLocal()) {
          const char *fname = VisData.fileName(theTask->taskRec->srcFile());
          snprintf (tmp, sizeof(tmp), "%f C %ld G %ld P %ld OC\n%s:%ld",
                    theTask->taskClock, theTask->commSum.numGets,
                    theTask->commSum.numPuts, theTask->commSum.numForks,
                    (fname[0] == '$' ? &fname[11] : fname),
                    theTask->taskRec->srcLine());
        } else {
          snprintf (tmp, sizeof(tmp), "%f C %ld G %ld P %ld OC",
                    theTask->taskClock, theTask->commSum.numGets,
                    theTask->commSum.numPuts, theTask->commSum.numForks);
        }
        btn->copy_tooltip(tmp);
        btn->callback(taskCallback, (void *)theTask);
      }
      if (curCol != col)
        greedy[col] = 0;
      curCol++;
    }
  if (curCol)
    curLine++;

  done = false; 

  while (!done && tl_itr !=  VisData.taskTimeline[parent->localeNum].end()) {

    switch (tl_itr->first) {
      
      case DataModel::Tl_Tag:
        if (parent->tagNum != DataModel::TagALL) {
          done = true;
          break;
        }
        curCol = 0;
        tmpTagNo = tl_itr->second;
        while (greedy[curCol] != 0) curCol++;
        snprintf (tmp, sizeof(tmp), "TAG: %s", 
                  VisData.getTagData(tmpTagNo)->name);
        b = new Fl_Box(FL_BORDER_BOX, x()+10+60*curCol, y()+40+25*curLine,
                               8+7*strlen(tmp), 20, NULL);
        b->copy_label(tmp);
        add(b);
        curLine++;
        break;

      case DataModel::Tl_Begin:
        curCol = 0;
        while (greedy[curCol] != 0) curCol++;
        greedy[curCol] = tl_itr->second; // store the task id
        greedyStart[curCol] = curLine;
        theTask = VisData.getTaskData(parent->localeNum,tl_itr->second, tmpTagNo);
        btn = new Fl_Button(x()+10+60*curCol, y()+40+25*curLine, 70, 20, NULL);
        btn->box(FL_ROUNDED_BOX);
        btn->down_box(FL_ROUNDED_BOX);
        snprintf (tmp, sizeof(tmp), "%c%s %ld",
                  theTask->commSum.numComms > 0 ? '*' : ' ',
                  theTask->taskRec->isLocal() ? "L" : "OC", tl_itr->second);
        btn->copy_label(tmp);
        btn->color(heatColor(theTask->taskClock,
                             VisData.getTagData(parent->tagNum)->
                             locales[parent->localeNum].maxTaskClock));
        btn->down_color(btn->color());
        btn->callback(taskCallback, (void *)theTask);
        add(btn);
        if (theTask->taskRec->isLocal()) {
          const char *fname = VisData.fileName(theTask->taskRec->srcFile());
          snprintf (tmp, sizeof(tmp), "%f C %ld G %ld P %ld OC\n%s:%ld",
                    theTask->taskClock, theTask->commSum.numGets,
                    theTask->commSum.numPuts, theTask->commSum.numForks,
                    (fname[0] == '$' ? &fname[11] : fname),
                    theTask->taskRec->srcLine());
        } else {
          snprintf (tmp, sizeof(tmp), "%f C %ld G %ld P %ld OC",
                    theTask->taskClock, theTask->commSum.numGets,
                    theTask->commSum.numPuts, theTask->commSum.numForks);
        }
        btn->copy_tooltip(tmp);
        curLine++;
        break;

      case DataModel::Tl_End:
        curCol = 0;
        while (greedy[curCol] != tl_itr->second) curCol++;
        // Update drawing information
        lineDrawData.startLine = greedyStart[curCol];
        lineDrawData.column = curCol;
        lineDrawData.endLine = curLine;
        lineDrawData.isEndTask = true;
        drawDB.push_back(lineDrawData);
        greedy[curCol] = 0;
        curLine++;
        break;
    }    
    tl_itr++;
  } 

  // Update drawing information for any element still in greedy
  lineDrawData.endLine = curLine;
  lineDrawData.isEndTask = false;
  for (int col = 0; col < progMaxConc; col++)
    if (greedy[col] != 0) {
      lineDrawData.column = col;
      lineDrawData.startLine = greedyStart[col];
      drawDB.push_back(lineDrawData);
    }

  redraw();
  
}
