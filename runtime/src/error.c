#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "chplrt.h"


char* _chpl_input_filename;
int   _chpl_input_linenumber;

void printError(char* message) {
  fflush(stdout);
  if (_chpl_input_linenumber) {
    fprintf(stderr, "%s, line%d:  ***Error: %s***\n", _chpl_input_filename, 
            _chpl_input_linenumber, message);
  } else {
    fprintf(stderr, "***Error: %s***\n", message);
  }
  exit(0);
}


void printWarning(char* message) {
  fflush(stdout);
  if (_chpl_input_linenumber) {
    fprintf(stderr, "%s, line%d:  ***Warning: %s***\n", _chpl_input_filename, 
            _chpl_input_linenumber, message);
  } else {  
    fprintf(stderr, "***Warning: %s***\n", message);
  }
}


void printInternalError(char* message) {
  fflush(stdout);
  fprintf(stderr, "***Internal Error: %s***\n", message);
  exit(1);
}


