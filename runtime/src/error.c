/*
 * Copyright 2004-2016 Cray Inc.
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

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#ifndef LAUNCHER
#include "chpl-atomics.h"
#endif

#ifdef CHPL_UNWIND_LIBUNWIND
// Necessary for instruct libunwind to use only the local unwind
#define UNW_LOCAL_ONLY
#include <libunwind.h>

static void chpl_stack_unwind(void){
  // This is just a prototype using libunwind
  unw_cursor_t cursor;
  unw_context_t uc;
  unw_word_t wordValue;
  char buffer[128];
  char *p;

  // Check if we need to print the stack trace (default = yes)
  if((p = getenv("CHPL_RT_UNWIND")) != NULL ){
    if(strcmp(p, "0") == 0)
      return;
  }

  unw_getcontext(&uc);
  unw_init_local(&cursor, &uc);

  if(chpl_sizeSymTable > 0)
    fprintf(stderr,"\nStacktrace\n\n");

  // This loop does the effective stack unwind, see libunwid documentation
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
        fprintf(stderr,"%s (%s:%d)\n",
                 chpl_funSymTable[t+1],
                 chpl_lookupFilename(chpl_filenumSymTable[t]),
                 chpl_filenumSymTable[t+1]);
        break;
      }
    }
  }
  fprintf(stderr,"\n");
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

#ifdef CHPL_UNWIND_LIBUNWIND
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


void chpl_msg(int verbose_level, const char* fmt, ...) {
  if (verbosity >= verbose_level) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
    fflush(stdout);
  }
}
