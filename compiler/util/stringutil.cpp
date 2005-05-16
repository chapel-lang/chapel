#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "chplalloc.h"
#include "misc.h"
#include "stringutil.h"

char* glomstrings(int numstrings, ...) {
  int i;

  char** stringlist = (char**)MALLOC(numstrings * sizeof(char*));
  va_list ap;
  va_start(ap, numstrings);
  for (i=0; i<numstrings; i++) {
    stringlist[i] = va_arg(ap, char*);
  }
  va_end(ap);

  unsigned int totlen = 0;
  for (i=0; i<numstrings; i++) {
    totlen += strlen(stringlist[i]);
  }

  char* newstring = (char*)MALLOC((totlen + 1)*sizeof(char));
  newstring[0] = '\0';
  for (i=0; i<numstrings; i++) {
    strcat(newstring, stringlist[i]);
  }

  if (strlen(newstring) > totlen) {
    INT_FATAL("glomstrings() buffer overflow");
  }

  return newstring;
}


char* copystring(char* str) {
  return glomstrings(1, str);
}


char* intstring(int i) {
  char scratch[8];

  sprintf(scratch, "%d", i);
  return glomstrings(1, scratch);
}
