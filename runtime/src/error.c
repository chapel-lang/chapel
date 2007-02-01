#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "chplrt.h"

void printError(char* message, _int32 lineno, _string filename) {
  fflush(stdout);
  if (lineno)
    fprintf(stderr, "%s, line%d:  ***Error: %s***\n", filename, lineno, message);
  else
    fprintf(stderr, "***Error: %s***\n", message);
  exit(0);
}

void printInternalError(char* message) {
  fflush(stdout);
  fprintf(stderr, "***Internal Error: %s***\n", message);
  exit(1);
}
