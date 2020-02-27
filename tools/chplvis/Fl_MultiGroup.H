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

/* Implementation of a "Multi-group", similar to a Tab set but without
 * the tabs.  Control of which group is visible is one of several methods
 *  that select the group to be displayed.
 */

#ifndef Fl_MultiGroup_H
#define Fl_MultiGroup_H

#include <FL/Fl_Group.H>

class Fl_MultiGroup : public Fl_Group {

  struct ll {
    Fl_Widget *child;
    bool removeAtPop;
    struct ll *next;
  } *childStack;
  int nOnStack;

  int numChildren;
  int selectedChild;

 protected:

public:

  Fl_MultiGroup (int x, int y, int w, int h, const char *l = 0);
  
  bool selectChild (int which);    // Select a child to be visible
  bool selectChild (Fl_Widget *w);

  bool pushChild (int which);      // Select a child to be visible and stacks current child
  bool pushChild (Fl_Widget *w);
  bool pushNewChild (Fl_Widget *w, bool removeAtPop);
  
  bool popChild ();                // pops a child, displays stacked child
  void popAll();                   // pops all children, displays bottom of stack

  int numOnStack() { return nOnStack; }

  int currentChild ();             // returns a child

  void add (Fl_Widget &o);         // Add a child
  void add (Fl_Widget *o);         // Add a child

  void insert (Fl_Widget &o, int number);  // Insert a child at specific index

  // Support for adding in the constructor for a widget
  void begin();
  void end();

};


#endif
