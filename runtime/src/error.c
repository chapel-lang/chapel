#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "chplexit.h"
#include "chplrt.h"

int verbosity = 1;

void _printError(const char* message, _int32 lineno, _string filename) {
  fflush(stdout);
  if (lineno)
    fprintf(stderr, "%s:%d: error: %s\n", filename, lineno, message);
  else
    fprintf(stderr, "error: %s\n", message);
  _chpl_exit(1);
}


void _printInternalError(const char* message) {
  fflush(stdout);
  fprintf(stderr, "internal error: %s\n", message);
  _chpl_exit(2);
}


void _chpl_msg(int verbose_level, const char* fmt, ...) {
  if (verbosity >= verbose_level) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
    fflush(stdout);
  }
}
