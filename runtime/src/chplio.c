#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chplio.h"
#include "chplrt.h"
#include "domain.h"
#include "error.h"


CFILEPTR _NULLCFILEPTR = NULL;

int _string_to_boolean(char* valueString, _boolean* val) {
  int validBoolean = 0;

  if (valueString) {
    if (strcmp(valueString, "true") == 0) {
      *val = true;
      validBoolean = 1;
    } else if (strcmp(valueString, "false") == 0){
      *val = false;
      validBoolean = 1;
    }
  }
  return validBoolean;
}

void _chpl_write_boolean(_boolean val) {
  printf(val ? "true" : "false");
}

void _chpl_write_integer(_integer64 val) {
  printf("%lld", val);
}

void _chpl_write_float(_float64 val) {
  const int floatBuffLen = 1024;
  char buff[floatBuffLen];
  sprintf(buff, "%g", val);
  if (strlen(buff) > floatBuffLen-1) {
    fprintf(stderr, "Error: float I/O buffer overrun\n");
    exit(1);
  }
  if (strchr(buff, '.') == NULL && strchr(buff, 'e') == NULL) {
    printf("%s.0", buff);
  } else {
    printf("%s", buff);
  }
}

void _chpl_write_string(_string val) {
  printf("%s", val);
}

void _chpl_write_nil(void* val) {
  printf("nil");
}

void _chpl_write_linefeed(void) {
  printf("\n");
}

void _chpl_read_boolean(_boolean* val) {
  char* inputString = NULL;
  int validBoolean = 0;

  _chpl_read_string(&inputString);
  validBoolean = _string_to_boolean(inputString, val);

  if (!validBoolean) {
    char* message = "Not of boolean type";
    printError(message);
  }
}

void _chpl_read_integer(_integer64* val) {
  scanf("%lld", val);
}

void _chpl_read_float(_float64* val) {
  scanf("%lg", val);
}

void _chpl_read_string(_string* val) {
  char localVal[_default_string_length];
  char dsl[1024];

  scanf("%255s", localVal);
  if (strlen(localVal) == (_default_string_length - 1)) {
    sprintf(dsl, "%d", _default_string_length);
    char* message = _glom_strings(2, "The maximum string length is ", dsl);
    printError(message);
  }
  _copy_string(val, localVal);
}


void _chpl_read_complex(_complex128** val) {
  char imaginaryI = 'i';
  int numScans;
  /* 
     NOTE:  The imaginary "i" is stored and checked in the if statement below
     to ensure that it has been entered directly after the second double of 
     the complex number, with no intervening space.
  */
  numScans =
    scanf("%lg  +  %lg%c", &((*val)->re), &((*val)->im), &imaginaryI);
  if ((imaginaryI != 'i') || (numScans < 2)) {
    char* message = "Incorrect format for complex numbers";
    printError(message);
  }
}
