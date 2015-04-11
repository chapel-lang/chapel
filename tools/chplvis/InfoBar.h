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

#ifndef INFOBAR_H
#define INFOBAR_H

#include <FL/Fl.H>
#include <FL/Fl_Box.H>

class InfoBar : public Fl_Box {

 private:
  int maxTasks;
  int maxComms;

 public:

  InfoBar (int x, int y, int w, int h, const char *label = 0)
    : Fl_Box(x,y,w,h) { maxTasks = 0; maxComms = 0; };
   
  void draw(void);

  void setMaxes(int tasks, int comms) {
    maxTasks = tasks;
    maxComms = comms;
  }

};


#endif

