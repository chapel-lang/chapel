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

#ifndef DATAMODEL_H
#define DATAMODEL_H

#include "Event.h"
#include <list>

class DataModel {

 private:

   int LoadFile (const char *filename, int index, double seq);

   void newList ();

 public:

  // 
   DataModel() { numLocales = -1; curEvent = theEvents.begin(); }

  //  LoadFile loads data from a collection of files
  //  filename of the form  basename-n, where n can
  //  be a multi-digit number
  //  Returns 1 if successful, 0 if not


  int LoadData (const char *filename);

  //  Number of locales found in loading files

  int NumLocales () { return numLocales; }

  int NumEvents () { return theEvents.size(); }

  Event * getFirstEvent() {
            curEvent = theEvents.begin();
	    if (curEvent == theEvents.end())
	      return NULL;
	    return *curEvent;
          }

  Event * getNextEvent() {
            curEvent++;
	    if (curEvent == theEvents.end())
	      return NULL;
	    return *curEvent;
          }

  int NumTags () { return numTags; }

 private:

  int numLocales;
  int numTags;

  std::list<Event*> theEvents;
  std::list<Event*>::iterator curEvent;

};


#endif
