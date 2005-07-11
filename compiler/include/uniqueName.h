#ifndef _UNIQUE_NAME_H_
#define _UNIQUE_NAME_H_
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

class UniqueName
{
  const char* name;
  const char* start; //the initial string (e.g. _u)
  char* newchar; //the character to add (e.g. underscore)
  char* newstr; //the unique sequence 
  void inc();
  
public:
  UniqueName(const char* n, const char* s, char* c) :
  name(n),
  start(s),
  newchar(c)
  {
    newstr = (char*) malloc((strlen(start) + 1)*sizeof(char));
    newstr = strcpy(newstr, start);
  }

  void resetStr();
  char* uniqueStr();
  char* uniqueStr(char* another);
  char* newName();
  virtual ~UniqueName() {
    delete newstr;
    newstr = NULL;
  }
};

#endif 
