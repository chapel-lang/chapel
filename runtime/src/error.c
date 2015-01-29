#include "chplrt.h"

#include "error.h"
#include "chplexit.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#ifndef LAUNCHER
#include "chpl-atomics.h"
#endif

int verbosity = 1;

void chpl_warning(const char* message, int32_t lineno, c_string filename) {
  // squash warnings if --quiet flag is used
  if (verbosity == 0) {
    return;
  }
  fflush(stdout);
  if (lineno > 0)
    fprintf(stderr, "%s:%" PRId32 ": warning: %s\n", filename, lineno, message);
  else if (filename)
    fprintf(stderr, "%s: warning: %s\n", filename, message);
  else
    fprintf(stderr, "warning: %s\n", message);
}


#ifndef LAUNCHER
static atomic_flag thisLocaleAlreadyExiting;
void chpl_error_init(void) {
  atomic_init_flag(&thisLocaleAlreadyExiting, false);
}
#endif

static void spinhaltIfAlreadyExiting(void) {
#ifndef LAUNCHER
  volatile int temp;
  if (atomic_flag_test_and_set(&thisLocaleAlreadyExiting)) {
    // spin forever if somebody else already set it to 1
    temp = 1;
    while (temp);
  }
#endif
}

static void chpl_error_common(const char* message, int32_t lineno, c_string filename) {
  spinhaltIfAlreadyExiting();
  fflush(stdout);
  if (lineno > 0)
    fprintf(stderr, "%s:%" PRId32 ": error: %s", filename, lineno, message);
  else if (filename)
    fprintf(stderr, "%s: error: %s", filename, message);
  else
    fprintf(stderr, "error: %s", message);
}


void chpl_error(const char* message, int32_t lineno, c_string filename) {
  chpl_error_common(message, lineno, filename);
  fprintf(stderr, "\n");
  chpl_exit_any(1);
}


void chpl_internal_error(const char* message) {
  spinhaltIfAlreadyExiting();
  fflush(stdout);
  fprintf(stderr, "internal error: %s\n", message);
  chpl_exit_any(2);
}


void chpl_msg(int verbose_level, const char* fmt, ...) {
  if (verbosity >= verbose_level) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
    fflush(stdout);
  }
}
