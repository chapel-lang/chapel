#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chplio.h"
#include "chplrt.h"
#include "domain.h"
#include "error.h"


CFILEPTR _NULLCFILEPTR = NULL;
CFILEPTR _STDINCFILEPTR;
CFILEPTR _STDOUTCFILEPTR;
CFILEPTR _STDERRCFILEPTR;

void initChplio(void) {
  _STDINCFILEPTR = stdin;
  _STDOUTCFILEPTR = stdout;
  _STDERRCFILEPTR = stderr;  
}


static void printfError(void) {
  char* message = _glom_strings(2, "Write failed: ", strerror(errno));
  printError(message);
}


static void scanfError(void) {
  char* message = _glom_strings(2, "Read failed: ", strerror(errno));
  printError(message);
}


void _chpl_fwrite_float_help(FILE* fp, _float64 val) {
  const int floatBuffLen = 1024;
  char buff[floatBuffLen];
  int returnVal = 0;
  returnVal = sprintf(buff, "%g", val);
  if (returnVal < 0) {
    printfError();
  }
  if (strlen(buff) > floatBuffLen-1) {
    fprintf(stderr, "Error: float I/O buffer overrun\n");
    exit(1);
  }
  if (strchr(buff, '.') == NULL && strchr(buff, 'e') == NULL) {
    returnVal = fprintf(fp, "%s.0", buff);
    if (returnVal < 0) {
      printfError();
    }
  } else {
    returnVal = fprintf(fp, "%s", buff);
    if (returnVal < 0) {
      printfError();
    }
  }
}


void _chpl_fread_string_help(FILE* fp, _string* val) {
  char localVal[_default_string_length];
  char dsl[1024];
  int returnVal = 0;
  returnVal = fscanf(fp, "%255s", localVal);
  if (returnVal < 0) {
    scanfError();
  }
  if (strlen(localVal) == (_default_string_length - 1)) {
    sprintf(dsl, "%d", _default_string_length);
    char* message = _glom_strings(2, "The maximum string length is ", dsl);
    printError(message);
  }
  _copy_string(val, localVal);
}


int _readLitChar(FILE* fp, _string val, int ignoreWhiteSpace) {
  int returnVal  = 0;
  char inputVal  = ' ';
  int charsMatch = 0;

  if (ignoreWhiteSpace) {
    while ((inputVal == ' ') || (inputVal == '\n') || (inputVal == '\r') || 
           (inputVal == '\t')) {
      returnVal = fscanf(fp, "%c", &inputVal);
    }
  } else {
    returnVal = fscanf(fp, "%c", &inputVal);
  }

  if (inputVal == *val) {
    charsMatch = 1;
  } else { 
    returnVal = ungetc(inputVal, fp);
    if (returnVal == EOF) {
      char* message = _glom_strings(2, "ungetc in _readLitChar failed: ", strerror(errno));
      printInternalError(message);
    }
  }
  return charsMatch;
}
