#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chplmem.h"
#include "chplrt.h"
#include "chpltypes.h"

void* nil = (void*)0x01;

char* _glom_strings(int numstrings, ...) {
  static int maxNumStrings = 0;
  static char** stringlist = NULL;
  int i;

  if (numstrings > maxNumStrings) {
    maxNumStrings = numstrings;
    stringlist = (char**)_chpl_realloc(stringlist, numstrings, sizeof(char*),
                                       "stringlist buffer in _glom_strings");
  }

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
                                        "_glom_strings result");
  newstring[0] = '\0';
  for (i=0; i<numstrings; i++) {
    strcat(newstring, stringlist[i]);
  }

  if (strlen(newstring) > totlen) {
    fprintf(stderr, "INTERNAL ERROR: _glom_strings() buffer overflow");
    exit(1);
  }

  return newstring;
}


void _copy_string(_string* lhs, _string rhs) {
  char* description = _glom_strings(3, "string copy of \"", rhs, "\"");
  *lhs = (char*)_chpl_realloc(*lhs, (strlen(rhs)+1), sizeof(char), description);
  _chpl_free(description);

  strcpy(*lhs, rhs);
}


char* _chpl_tostring_boolean(_boolean x, char* format) {
  if (x) {
    return _glom_strings(1, "true");
  } else {
    return _glom_strings(1, "false");
  }
}


char* _chpl_tostring_integer(_integer64 x, char* format) {
  char buffer[256];
  sprintf(buffer, format, x);
  return _glom_strings(1, buffer);
}


char* _chpl_tostring_float(_float64 x, char* format) {
  char buffer[256];
  sprintf(buffer, format, x);
  return _glom_strings(1, buffer);
}


char* _chpl_tostring_complex(_complex128 x, char* format) {
  char buffer[256];
  sprintf(buffer, format, x);
  return _glom_strings(1, buffer);
}
