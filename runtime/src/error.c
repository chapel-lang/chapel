#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "chplrt.h"



void printError(char* message) {
  fflush(stdout);
  fprintf(stderr, "***Error: %s***\n", message);
  exit(0);
}


void printWarning(char* message) {
  fflush(stdout);
  fprintf(stderr, "***Warning: %s***\n", message);
}


void printInternalError(char* message) {
  fflush(stdout);
  fprintf(stderr, "***Internal Error: %s***\n", message);
  exit(1);
}


