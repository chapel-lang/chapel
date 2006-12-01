#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chplio.h"
#include "chplrt.h"
#include "error.h"

static void scanfError(void) {
  char* message = _glom_strings(2, "Read failed: ", strerror(errno));
  printError(message);
}

_string string_fscanf(FILE* fp) {
  char localVal[_default_string_length];
  char dsl[1024];
  int returnVal = 0;
  returnVal = fscanf(fp, "%255s", localVal);
  if (returnVal == EOF) {
    printError("Read failed: EOF");
  }
  if (returnVal < 0) {
    scanfError();
  }
  if (strlen(localVal) == (_default_string_length - 1)) {
    char* message;
    sprintf(dsl, "%d", _default_string_length);
    message = _glom_strings(2, "The maximum string length is ", dsl);
    printError(message);
  }
  return string_copy(localVal);
}

                            
_bool readLit(FILE* fp, _string val, _bool ignoreWhiteSpace) {
  int returnVal  = 0;
  char inputVal  = ' ';

  if (ignoreWhiteSpace) {
    while ((inputVal == ' ') || (inputVal == '\n') || (inputVal == '\r') || 
           (inputVal == '\t')) {
      returnVal = fscanf(fp, "%c", &inputVal);
    }
  } else {
    returnVal = fscanf(fp, "%c", &inputVal);
  }

  if (inputVal == *val) {
    return true;
  } else { 
    returnVal = ungetc(inputVal, fp);
    if (returnVal == EOF) {
      char* message = _glom_strings(2, "ungetc in _readLitChar failed: ", strerror(errno));
      printInternalError(message);
    }
  }
  return false;
}
