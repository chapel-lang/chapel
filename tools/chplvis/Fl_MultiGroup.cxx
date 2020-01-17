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

#include "Fl_MultiGroup.H"
#include <FL/Fl_Window.H>
#include <stdio.h>

Fl_MultiGroup::Fl_MultiGroup (int x, int y, int w, int h, const char *l)
  : Fl_Group(x, y, w, h, l) 
{
  childStack = NULL;
  numChildren = 0;
  nOnStack = 0;
  selectedChild = 0;
  begin();
}

bool Fl_MultiGroup::selectChild (int which)
{
  if (which < 0 || which >= children())
    return false;
  
  // Hide the current selected
  child(selectedChild)->hide();
  // Select the new child
  selectedChild = which;
  child(which)->show();
  window()->redraw();
  return true;
}

bool Fl_MultiGroup::selectChild (Fl_Widget *w)
{
  int which = find(w);
  if (which < children())
    return selectChild(which);
  else
    return false;
}

bool Fl_MultiGroup::pushChild (int which)
{
  if (which < 0 || which >= children())
    return false;

  struct ll* newEl = new struct ll;
  newEl->child = child(selectedChild);
  newEl->removeAtPop = false;
  newEl->next = childStack;
  if (selectChild(which)) {
    childStack = newEl;
    nOnStack++;
    return true;
  }
  delete newEl;
  window()->redraw();
  return false;
}

bool Fl_MultiGroup::pushChild (Fl_Widget *w)
{
  int which = find(w);
  if (which < children())
    return pushChild(which);
  return false;
}

bool Fl_MultiGroup::pushNewChild (Fl_Widget *w, bool removeAtPop)
{
  add(w);
  if (pushChild(w)) {
    childStack->removeAtPop = removeAtPop;
    return true;
  }
  if (removeAtPop) {
    remove(w);
  }
  return false;
}

bool Fl_MultiGroup::popChild ()
{
  struct ll* lastChild = childStack;
  int wasSel = selectedChild;
  if (childStack == NULL)
    return false;
  childStack = lastChild->next;
  nOnStack--;
  selectChild(lastChild->child);
  if (lastChild->removeAtPop) {
    //Fl_Widget *item = child(wasSel);
    remove (wasSel);
    //delete item;
  }
  delete lastChild;
  return true;
}

void Fl_MultiGroup::popAll()
{
  while (childStack != NULL) popChild();
}

int Fl_MultiGroup::currentChild ()
{ return selectedChild; }

void Fl_MultiGroup::add (Fl_Widget &o)
{
  int  X, Y, W, H;
   X = x();  Y = y();  W = w();  H = h();

  if (children() > 0) {
    o.hide();
  } else {
    o.show();
    selectedChild = 0;
  }
  o.resize(X,Y,W,H);
  Fl_Group::add(o);
}

void Fl_MultiGroup::add (Fl_Widget *o)
{
  if (children() > 0) {
    o->hide();
  } else {
    o->show();
    selectedChild = 0;
  }
  Fl_Group::add(o);
}

void Fl_MultiGroup::insert (Fl_Widget &o, int number)
{
  int  X, Y, W, H;
  X = x();  Y = y();  W = w();  H = h();
  if (number == selectedChild)
    child(selectedChild)->hide();
  else
    o.hide();
  o.resize(X,Y,W,H);
  Fl_Group::insert(o,number);
  child(selectedChild)->show();
}


// Widget construction support

void Fl_MultiGroup::begin() 
 {
   Fl_Group::current(this);
 }

void Fl_MultiGroup::end()
 {
   int i, X, Y, W, H;
   X = x();  Y = y();  W = w();  H = h();
   Fl_Group::current(parent());

   for (i = 0 ; i < children(); i++) {
     child(i)->resize(X,Y,W,H);
     child(i)->hide();
   }
   selectChild(0);
   window()->redraw();
 }

