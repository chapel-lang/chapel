#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringutil.h"

char* glomstrings(int numstrings, ...) {
  char* control = (char*)malloc((numstrings*2 + 1)*sizeof(char));
  int i;

  for (i=0; i<numstrings; i++) {
    control[2*i]     = '%';
    control[2*i + 1] = 's';
  }
  control[2*numstrings] = '\0';

  char** stringlist = (char**)malloc(numstrings * sizeof(char*));
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

  char* newstring = (char*)malloc((totlen + 1)*sizeof(char));
  newstring[0] = '\0';
  for (i=0; i<numstrings; i++) {
    strcat(newstring, stringlist[i]);
  }

  if (strlen(newstring) > totlen) {
    fprintf(stderr, "glomstrings() buffer overflow");
    exit(1);
  }

  return newstring;
}


char* copystring(char* str) {
  return glomstrings(1, str);
}
