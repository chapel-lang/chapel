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

// returns 0 for no error, and stores into dst a C string with
// the path to the running executable.
int chpl_getexe_path(char* dst, size_t max_dst)
{
#ifdef __linux__
  int ret;
  ret = readlink("/proc/self/exe", dst, max_dst);
  // return an error if there was an error.
  if( ret < 0 ) return -1;
  // append the NULL byte
  if( ret < max_dst ) dst[ret] = '\0';
  return 0;
#else
#ifdef __APPLE__
  uint32_t sz = max_dst;
  return _NSGetExecutablePath(dst, &sz);
#else
  // getexe path not available.
  return -1;
#endif
#endif
}

#ifdef __GLIBC__

#include <execinfo.h>

// we have backtrace and backtrace_symbols_fd functions
void chpl_exit_backtrace(int exit_code)
{
#define IGNORE_NEWEST 3
#define IGNORE_OLDEST 2
#define EXTRA_ARGS 7
#define MAX_STACK 32
  void* array[MAX_STACK];
  const char* addr_args[EXTRA_ARGS+MAX_STACK];
  char* tmp;
  char exepath[512];
  size_t sz;
  int i,j;
  int haspath;
  pid_t child_pid;
  int child_status;
  int printed_trace = 0;

  fprintf(stdout, "Stack trace:\n");
  fflush(stdout);
  fflush(stderr);

  memset(exepath, 0, sizeof(exepath));
  memset(addr_args, 0, sizeof(addr_args));

  haspath = ! chpl_getexe_path(exepath, sizeof(exepath));

  sz = backtrace(array, MAX_STACK);

  // ignore some (e.g. main.c, libc start, etc).
  sz -= IGNORE_OLDEST;

  if( haspath ) {
    j = 0;
    addr_args[j++] = "addr2line"; // argv[0] is always exe name.
    addr_args[j++] = "-s"; // basenames, not full paths
    addr_args[j++] = "-a"; // print addresses
    addr_args[j++] = "-i"; // show inlining nicely
    addr_args[j++] = "-p"; // "pretty-print"
    addr_args[j++] = "-e"; // giving executable path
    addr_args[j++] = exepath;

    for( i = IGNORE_NEWEST; i < sz; i++ ) {
      tmp = alloca(20); // 0x + 16 hex chars + \0
      snprintf(tmp, 20, "%p", array[i]);
      addr_args[j++] = tmp;
    }

    addr_args[j++] = NULL;

    child_pid = vfork();

    // run addr2line in the child.
    if( child_pid == 0 ) {
      // First, try running addr2line
      execvp(addr_args[0], (char* const *) &addr_args);
      // if we're still running... return an error.
      _exit(-1); // returning with _exit because exit is #defined to use_chpl_exit
    } else if( child_pid  > 0 ) {
      // Parent, wait for child to exit.
      waitpid(child_pid, &child_status, 0);
      if( child_status == 0 ) printed_trace = 1;
    }
  }

  if( !printed_trace ) {
    // print the stack trace with symbols
    // we could use backtrace_symbols, but this version
    // should work even in low-memory situations.
    backtrace_symbols_fd(array, sz, 2 /* 1 is stdout, 2 is stderr */);
  }

  chpl_exit_any(exit_code);
#undef MAX_STACK
}
#else
void chpl_exit_backtrace(int exit_code)
{
  fflush(stdout);
  fflush(stderr);

  fprintf(stderr, "backtrace not available\n");
  chpl_exit_any(exit_code);
}
#endif


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
    fprintf(stderr, "%s:%" PRId32 ": error: %s\n", filename, lineno, message);
  else if (filename)
    fprintf(stderr, "%s: error: %s\n", filename, message);
  else
    fprintf(stderr, "error: %s\n", message);
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
