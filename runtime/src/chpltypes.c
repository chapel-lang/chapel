#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chplmem.h"
#include "chplrt.h"
#include "chpltypes.h"

void* nil = (void*)0x01;

char* glomstrings(int numstrings, ...) {
  static int maxControlStrings = 0;
  static char* control = NULL;
  static char** stringlist = NULL;
  int i;

  if (numstrings > maxControlStrings) {
    maxControlStrings = numstrings;
    control = (char*)_chpl_realloc(control, (numstrings*2 + 1), sizeof(char),
				   "control string buffer in glomstrings");
    stringlist = (char**)_chpl_realloc(stringlist, numstrings, sizeof(char*),
				       "stringlist buffer in glomstrings");
  }

  for (i=0; i<numstrings; i++) {
    control[2*i]     = '%';
    control[2*i + 1] = 's';
  }
  control[2*numstrings] = '\0';

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

  char* newstring = (char*)_chpl_malloc((totlen + 1), sizeof(char), 
					"glomstrings result");
  newstring[0] = '\0';
  for (i=0; i<numstrings; i++) {
    strcat(newstring, stringlist[i]);
  }

  if (strlen(newstring) > totlen) {
    fprintf(stderr, "INTERNAL ERROR: glomstrings() buffer overflow");
    exit(1);
  }

  return newstring;
}


void _copy_string(_string* lhs, _string rhs) {
  char* description = glomstrings(3, "string copy of \"", rhs, "\"");
  *lhs = (char*)_chpl_realloc(*lhs, (strlen(rhs)+1), sizeof(char), description);
  _chpl_free(description);

  strcpy(*lhs, rhs);
}
