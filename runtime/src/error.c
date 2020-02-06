/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "chplrt.h"
#include "chpl-linefile-support.h"
#include "chplcgfns.h"

#include "error.h"
#include "chplexit.h"
#include "chpl-env.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#ifndef LAUNCHER
#include "chpl-atomics.h"
#ifdef CHPL_DO_UNWIND
#define CHPL_UNWIND_NOT_LAUNCHER
#endif
#endif

#ifdef CHPL_UNWIND_NOT_LAUNCHER
// Necessary for instruct libunwind to use only the local unwind
#define UNW_LOCAL_ONLY
#include <libunwind.h>

#ifdef __linux__
// We create a pipe with addr2line and try to get a line number
// Currently the precise line number works only on linux64
static int chpl_unwind_getLineNum(char *process_name, void *addr){
  char buf[128];
  int line;
  char* p;
  FILE *f;

  // We use a little POSIX script for avoiding the case in which
  // addr2line isn't present
  sprintf(buf,
          "if test -x /usr/bin/addr2line; then /usr/bin/addr2line -e %s %p;fi",
          process_name, addr);
  f = popen (buf, "r");
  if(f == NULL){
    // We wasn't able to start our pipe, we just give up
    return 0;
  }
  p = fgets(buf, sizeof(buf), f);
  if(p == NULL){
    // For some reason we wasn't able to read from the pipe, close and exit
    pclose(f);
    return 0;
  }
  pclose(f);
  // file name is until ':'
  while (*p++ != ':');
  line = atoi(p);

  return line;
}
#endif

static void chpl_stack_unwind(void){
  // This is just a prototype using libunwind
  unw_cursor_t cursor;
  unw_context_t uc;
  unw_word_t wordValue;
  char buffer[128];
  unsigned int line;

#ifdef __linux__
  unw_proc_info_t info;
  // Get the exec path and name for the precise line printing
  char process_name[128];

  line = readlink("/proc/self/exe", process_name, sizeof(process_name));
  // It unlikely to happen but this means that the process name is too big 
  // for our buffer. In this case, we truncate the name
  if(line == sizeof(process_name))
    line = sizeof(process_name)-1;
  process_name[line] = '\0';
#endif

  // Check if we need to print the stack trace (default = yes)
  if(! chpl_env_rt_get_bool("UNWIND", true)) {
    return;
  }

  line = 0;
  unw_getcontext(&uc);
  unw_init_local(&cursor, &uc);

  if(chpl_sizeSymTable > 0)
    fprintf(stderr,"Stacktrace\n\n");

  // This loop does the effective stack unwind, see libunwind documentation
  while (unw_step(&cursor) > 0) {
    unw_get_proc_name(&cursor, buffer, sizeof(buffer), &wordValue);
    // Since this stack trace is printed out a program exit, we do not believe
    // it is performance sensitive. Additionally, this initial implementation
    // favors simplicity over performance.
    //
    // If it becomes necessary to improve performance, this code could use be
    // faster using one of these two strategies:
    // 1) Use a hashtable or map to find entries in chpl_funSymTable, or
    // 2) Emit chpl_funSymTable in sorted order and use binary search on it
    for(int t = 0; t < chpl_sizeSymTable; t+=2 ){
      if (!strcmp(chpl_funSymTable[t], buffer)){
#ifdef __linux__
        // Maybe we can get a more precise line number
        unw_get_proc_info(&cursor, &info);
        line = chpl_unwind_getLineNum(process_name,
                                      (void *)(info.start_ip + wordValue));
        // We wasn't able to obtain the line number, let's use the procedure
        // line number
        if(line == 0)
          line = chpl_filenumSymTable[t+1];
#else
        line = chpl_filenumSymTable[t+1];
#endif
        fprintf(stderr,"%s() at %s:%d\n",
                  chpl_funSymTable[t+1],
                  chpl_lookupFilename(chpl_filenumSymTable[t]),
                  line);
        break;
      }
    }
  }
}
#endif

int verbosity = 1;

void chpl_warning(const char* message, int32_t lineno, int32_t filenameIdx) {
  const char* filename = NULL;
  // squash warnings if --quiet flag is used
  if (verbosity == 0) {
    return;
  }
  if (filenameIdx != 0)
    filename = chpl_lookupFilename(filenameIdx);
  chpl_warning_explicit(message, lineno, filename);
}

void chpl_warning_explicit(const char *message, int32_t lineno,
                           const char *filename) {
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
static atomic_bool thisLocaleAlreadyExiting;
void chpl_error_init(void) {
  atomic_init_bool(&thisLocaleAlreadyExiting, false);
}
#endif

static void spinhaltIfAlreadyExiting(void) {
#ifndef LAUNCHER
  volatile int temp;
  if (atomic_exchange_bool(&thisLocaleAlreadyExiting, true)) {
    // spin forever if somebody else already set it to 1
    temp = 1;
    while (temp);
  }
#endif
}

void chpl_error_explicit(const char *message, int32_t lineno,
                         const char *filename) {
  spinhaltIfAlreadyExiting();
  fflush(stdout);
  if (lineno > 0)
    fprintf(stderr, "%s:%" PRId32 ": error: %s", filename, lineno, message);
  else if (filename)
    fprintf(stderr, "%s: error: %s", filename, message);
  else
    fprintf(stderr, "error: %s", message);
  fprintf(stderr, "\n");

#ifdef CHPL_UNWIND_NOT_LAUNCHER
  chpl_stack_unwind();
#endif

  chpl_exit_any(1);
}


static
void msg_explicit_vs(char *restrict, size_t,
                     int32_t, const char *restrict,
                     const char *restrict,
                     const char *restrict, va_list)
       __attribute__((format(printf, 5, 0)));

static
void msg_explicit_vs(char *restrict str, size_t size,
                     int32_t lineno, const char *restrict filename,
                     const char *restrict severity,
                     const char *restrict format, va_list ap) {
  int len;

  if (lineno > 0)
    len = snprintf(str, size, "%s:%" PRId32 ": %s: ", filename, lineno,
                   severity);
  else if (filename)
    len = snprintf(str, size, "%s: %s: ", filename, severity);
  else
    len = snprintf(str, size, "%s: ", severity);

  if (len < size) {
    str += len;
    size -= len;

    len = vsnprintf(str, size, format, ap);

    if (len < size) {
      str[len] = '\n';
      str[len + 1] = '\0';
    }
  }
}


static
void msg_explicit_v(FILE*,
                    int32_t, const char *restrict,
                    const char *restrict,
                    const char *restrict, va_list)
       __attribute__((format(printf, 5, 0)));

static
void msg_explicit_v(FILE* f,
                    int32_t lineno, const char *restrict filename,
                    const char *restrict severity,
                    const char *restrict format, va_list ap) {
  char buf[1024];
  msg_explicit_vs(buf, sizeof(buf), lineno, filename, severity, format, ap);
  fputs(buf, f);
}


void chpl_error_preformatted(const char* message) {
  spinhaltIfAlreadyExiting();
  fflush(stdout);
  fprintf(stderr, "%s\n", message);

#ifdef CHPL_UNWIND_NOT_LAUNCHER
  chpl_stack_unwind();
#endif

  chpl_exit_any(1);
}

void chpl_error(const char *message, int32_t lineno, int32_t filenameIdx) {
  const char *filename = NULL;
  if (filenameIdx != 0)
    filename= chpl_lookupFilename(filenameIdx);
  chpl_error_explicit(message, lineno, filename);
}


void chpl_internal_error(const char* message) {
  spinhaltIfAlreadyExiting();
  fflush(stdout);
  fprintf(stderr, "internal error: %s\n", message);
  chpl_exit_any(2);
}


void chpl_internal_error_v(const char *restrict format, ...) {
  spinhaltIfAlreadyExiting();
  fflush(stdout);

  va_list ap;
  va_start(ap, format);
  msg_explicit_v(stderr, 0, NULL, "internal error", format, ap);
  va_end(ap);

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
