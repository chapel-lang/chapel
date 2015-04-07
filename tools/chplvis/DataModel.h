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
	    return *curEvent;
          }

  Event * getNextEvent() {
	    if (curEvent == theEvents.end())
	      return NULL;
	    else 
	      return *(++curEvent);
          }

 private:

  int numLocales;

  std::list<Event*> theEvents;
  std::list<Event*>::iterator curEvent;

};


#endif
