#ifndef _UNIQUE_NAME_H_
#define _UNIQUE_NAME_H_
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "chplalloc.h"

class UniqueName
{
  //const char* name;
  const char* start; //the initial string (e.g. _u)
  char* newchar; //the character to add (e.g. underscore)
  char* newstr; //the unique sequence 
  
public:
  UniqueName(const char* s, char* c) :
  start(s),
  newchar(c)
  {
    newstr = (char*) MALLOC((strlen(start) + 1)*sizeof(char));
    newstr = strcpy(newstr, start);
  }

  void inc();
  void resetStr();
  char* uniqueStr();
  char* uniqueStr(char* another);
  char* newName(char* id);
  virtual ~UniqueName() {
    delete newstr;
    newstr = NULL;
  }
};

#endif 
