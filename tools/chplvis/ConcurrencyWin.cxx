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

#include <stdio.h>
#include <assert.h>

#include "ConcurrencyWin.h"
#include "chplvis.h"

#include <FL/fl_draw.H>

ConcurrencyWin::ConcurrencyWin (int x, int y, int W, int H, const char *l)
  :  Fl_Double_Window (W, H, l)
{
}

void ConcurrencyWin::updateData (long loc, long tag)
{
  char tmp[2084];
  localeNum = loc;
  tagNum = tag;
  curTag = VisData.getTagData(tagNum);
  snprintf(tmp, sizeof(tmp), "Concurrency for Locale %ld, tag %s, max %ld",
           loc, curTag->name.c_str(), curTag->locales[loc].maxConc);
  title->copy_label(tmp);
  dataBox->buildData();
  redraw();
}

// Concurrency Data 
void ConcurrencyData::draw (void) {

  int s_w, s_h, s_xpos, s_ypos;  // Scroll information

  int l_x;   // Line x placement
  int l_y1;  // Line y start
  int l_y2;  // Line y stop
  bool y2_clip;  // Did y2 get clipped

  Fl_Group::draw();
  std::list<drawData>::iterator dbItr;

  // Get the scroll window information for "clipping", e.g. not drawing
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

#if 0
    fl_color(FL_RED);
    fl_line_style(FL_SOLID, 3, NULL);
    fl_line(1, 33, 100, 33);
#endif

    // Draw vertical line
    fl_color(FL_BLACK);
    fl_line_style(FL_SOLID, 3, NULL);
    fl_line(l_x, l_y1+30, l_x, l_y2+30);

    if (dbItr->isEndTask && !y2_clip) {
      // Draw the end of task line
      fl_line(l_x-5, l_y2+30, l_x+5, l_y2+30);
    }
  }
  fl_line_style(FL_SOLID, 1, NULL);
}

int ConcurrencyData::handle(int event) {
  return Fl_Group::handle(event);
}


void ConcurrencyData::buildData(void) {
  Fl_Box *b;
  char tmp[2048];
  int width = w();
  int height = h();
  int progMaxConc =  VisData.getTagData(DataModel::TagALL)->maxConc;
  int greedy[progMaxConc];
  int greedyStart[progMaxConc];
  int numLines = 0;
  int curLine;
  int curCol;

  DataModel::tagData *tmpTag;
  long tmpTagNo;

  std::list <DataModel::timelineEntry>::iterator tl_itr;
  std::list <DataModel::timelineEntry>::iterator tagStart;

  taskData theTask;
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

  // Special case for locale 0 .. main thread of 1
  if (parent->localeNum == 0)
    greedy[0] = 1;

  // Calculate new height
  if (parent->tagNum == DataModel::TagALL) {
    numLines =  VisData.taskTimeline[parent->localeNum].size();
    width = 30 + 60 * VisData.getTagData(DataModel::TagALL)->
                      locales[parent->localeNum].maxConc;
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
          //tmpTag = VisData.getTagData(tmpTagNo);
          if (tmpTagNo == parent->tagNum) {
            tagStart = tl_itr;
            tagStart++; // Move to the first record after the tag
            width = 30 + 60 * tmpTag->locales[parent->localeNum].maxConc;
          }
          break;

        case DataModel::Tl_Begin:
          if (tmpTagNo < parent->tagNum) {
            theTask =  VisData.getTaskData(parent->localeNum, tl_itr->second, tmpTagNo);
            if (theTask.taskRec != NULL && theTask.endTagNo != tmpTagNo) {
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
              if (theTask.taskRec == NULL) 
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
                                   
  //printf ("data size:  %dx%d\n", width, height);
  
  // Start the rebuild by resizing
  resize (x(), y(), width, height);

  // Reset scroll
  parent->scroll->scroll_to(0,0);

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
      greedy[curCol] = greedy[col];
      greedyStart[curCol] = 0;
      b = new Fl_Box::Fl_Box(FL_BORDER_BOX, 15+60*curCol, 40, 60, 20, NULL);
      if (parent->localeNum == 0 && greedy[col] == 1)
        snprintf (tmp, sizeof(tmp), "Main");
      else
        snprintf (tmp, sizeof(tmp), "C%d", greedy[curCol]);
      b->copy_label(tmp);
      add(b);
      theTask = VisData.getTaskData(parent->localeNum, greedy[curCol]);
      if (theTask.taskRec && theTask.taskRec->isLocal()) {
        snprintf (tmp, sizeof(tmp), "%s:%ld", theTask.taskRec->srcName(),
                  theTask.taskRec->srcLine());
        b->copy_tooltip(tmp);
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
        while (greedy[curCol] != 0) curCol++;
        snprintf (tmp, sizeof(tmp), "TAG: %s", 
                  VisData.getTagData(tl_itr->second)->name.c_str());
        b = new Fl_Box::Fl_Box(FL_BORDER_BOX, 10+60*curCol, 40+25*curLine,
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
        b = new Fl_Box::Fl_Box(FL_ROUNDED_BOX, 10+60*curCol,
                               40+25*curLine, 70, 20, NULL);
        snprintf (tmp, sizeof(tmp), "%c %ld", 
                   theTask.taskRec->isLocal() ? 'L' : 'F', tl_itr->second);
        b->copy_label(tmp);
        add(b);
        if (theTask.taskRec->isLocal()) {
          snprintf (tmp, sizeof(tmp), "%s:%ld", theTask.taskRec->srcName(),
                    theTask.taskRec->srcLine());
          b->copy_tooltip(tmp);
        }
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

#if 0
  // Build the task box
  b = new Fl_Box::Fl_Box(FL_ROUNDED_BOX, 10, 40, 70, 20, NULL);
  b->copy_label("testing");
  add(b);
  snprintf (tmp, sizeof(tmp), "Data: %d", 127853);
  b->copy_tooltip(tmp);
#endif

  redraw();
  
}
