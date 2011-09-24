#include "error.h"
#include "chplexit.h"
#include "chplrt.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

// fork, execvp, waitpid
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int verbosity = 1;
_Bool gDoBacktrace = false;

void chpl_set_backtrace_config(_Bool doBacktrace)
{
  // doBacktrace is the value of the config const;
  // we alse will do backtracing if either GASNET_BACKTRACE
  // or CHPL_BACKTRACE are set.
  if( doBacktrace ||
      gasnett_getenv_yesno_withdefault("GASNET_BACKTRACE", 0) ||
      gasnett_getenv_yesno_withdefault("CHPL_BACKTRACE", 0) ) {
    gDoBacktrace = true;
  } else {
    gDoBacktrace = false;
  }
}

void chpl_exit_backtrace(int exit_code)
{
  fflush(stdout);
  fflush(stderr);
  if( gDoBacktrace ) gasnett_print_backtrace(fileno(stderr));
  //gasnett_print_backtrace_ifenabled(fileno(stderr));
  gasnett_freezeForDebuggerErr();
  chpl_exit_any(exit_code);
}

void chpl_warning(const char* message, int32_t lineno, chpl_string filename) {
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

static volatile int thisLocaleAlreadyExiting = 0;

static void spinhaltIfAlreadyExiting(void) {
  volatile int temp;
  if (thisLocaleAlreadyExiting == 0) {
    thisLocaleAlreadyExiting = 1;
    return;
  }
  // spin forever if somebody else already set it to 1
  temp = 1;
  while (temp);
}

void chpl_error(const char* message, int32_t lineno, chpl_string filename) {
  spinhaltIfAlreadyExiting();
  fflush(stdout);
  if (lineno > 0)
    fprintf(stderr, "%s:%" PRId32 ": error: %s\n", filename, lineno, message);
  else if (filename)
    fprintf(stderr, "%s: error: %s\n", filename, message);
  else
    fprintf(stderr, "error: %s\n", message);
  chpl_exit_backtrace(1);
}

void chpl_error_noexit(const char* message, int32_t lineno, chpl_string filename) {
  spinhaltIfAlreadyExiting();
  fflush(stdout);
  if (lineno > 0)
    fprintf(stderr, "%s:%" PRId32 ": error: %s", filename, lineno, message);
  else if (filename)
    fprintf(stderr, "%s: error: %s", filename, message);
  else
    fprintf(stderr, "error: %s", message);
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
