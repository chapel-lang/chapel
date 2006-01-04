#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "chplrt.h"


char* chpl_input_filename = "";
int   chpl_input_lineno = 0;

void printError(char* message) {
  fflush(stdout);
  if (chpl_input_lineno) {
    fprintf(stderr, "%s, line%d:  ***Error: %s***\n", chpl_input_filename, 
            chpl_input_lineno, message);
  } else {
    fprintf(stderr, "***Error: %s***\n", message);
  }
  exit(0);
}


void printWarning(char* message) {
  fflush(stdout);
  if (chpl_input_lineno) {
    fprintf(stderr, "%s, line%d:  ***Warning: %s***\n", chpl_input_filename, 
            chpl_input_lineno, message);
  } else {  
    fprintf(stderr, "***Warning: %s***\n", message);
  }
}


void printInternalError(char* message) {
  fflush(stdout);
  fprintf(stderr, "***Internal Error: %s***\n", message);
  exit(1);
}


