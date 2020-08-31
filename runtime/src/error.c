/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#define _GNU_SOURCE
// needed for dlfcn.h on linux

#include "chplrt.h"
#include "chpl-linefile-support.h"
#include "chplcgfns.h"

#include "error.h"
#include "chplexit.h"
#include "chpl-mem.h"
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
#include <dlfcn.h> // for dladdr
// We create a pipe with addr2line and try to get a line number
// Currently the precise line number works only on linux64
static int chpl_unwind_getLineNum(void *addr){

  int rc;
  Dl_info info;
  intptr_t relativeAddr;
  char buf[1024];
  int i = 0;
  int line;
  char* p;
  FILE *f;
  ssize_t path_len;

  // We use a little shell script for avoiding the case in which
  // addr2line isn't present
  const char* scriptPreArgs =
    "if test -x /usr/bin/addr2line; then /usr/bin/addr2line -e ";
  // then the path
  // then space
  // then the address
  // then
  const char* scriptPostArgs = "; fi";

  // Start the buffer out with the script
  memcpy(buf, scriptPreArgs, strlen(scriptPreArgs));
  i = strlen(scriptPreArgs);

  // Compute the object containing the address
  rc = dladdr(addr, &info);
  if (rc == 0)
    return 0; // dladdr failed.

  // Compute the relative address within the object
  relativeAddr = (intptr_t)addr - (intptr_t)info.dli_fbase;

  // Compute the path to the file containing the object
  if (info.dli_fname != NULL && info.dli_fname[0] != '\0') {
    // use the path from dladdr
    path_len = strlen(info.dli_fname);
    if (i+path_len >= sizeof(buf))
      return 0; // not enough room in buffer - give up
    memcpy(&buf[i], info.dli_fname, path_len);
  } else {
    // Try using the file path from the current executable
    path_len = readlink("/proc/self/exe", &buf[i], sizeof(buf)-i);
    if (path_len >= sizeof(buf)-i)
      return 0; // truncation occured - give up.
    if (path_len == -1)
      return 0; // readlink returned error - give up.
  }
  i += path_len;

  rc = snprintf(&buf[i], sizeof(buf)-i,
                " %p%s", (void*)relativeAddr, scriptPostArgs);
  if (rc+1 >= sizeof(buf)-i)
    return 0; // command too long for buffer - give up

  f = popen(buf, "r");
  if (f == NULL)
    return 0; // popen failed - give up

  p = fgets(buf, sizeof(buf), f);
  if (p == NULL){
    // couldn't read from the pipe - close and give up
    pclose(f);
    return 0;
  }
  pclose(f);
  // file name is until ':'
  while (*p++ != ':') { }
  line = atoi(p);

  return line;
}
#endif

void chpl_stack_unwind(FILE* out, char sep) {
  // This is just a prototype using libunwind
  unw_cursor_t cursor;
  unw_context_t uc;
  unw_word_t wordValue;
  char buffer[256];
  unsigned int line;

  // Check if we need to print the stack trace (default = yes)
  if(! chpl_env_rt_get_bool("UNWIND", true)) {
    return;
  }

  line = 0;
  unw_getcontext(&uc);
  unw_init_local(&cursor, &uc);

  if(chpl_sizeSymTable > 0)
    fprintf(out,"Stacktrace%c%c", sep, sep);

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
        unw_proc_info_t info;
        // Maybe we can get a more precise line number
        unw_get_proc_info(&cursor, &info);
        line = chpl_unwind_getLineNum((void *)(info.start_ip + wordValue));
        // We wasn't able to obtain the line number, let's use the procedure
        // line number
        if(line == 0)
          line = chpl_filenumSymTable[t+1];
#else
        line = chpl_filenumSymTable[t+1];
#endif
        fprintf(out,"%s() at %s:%d%c",
                  chpl_funSymTable[t+1],
                  chpl_lookupFilename(chpl_filenumSymTable[t]),
                  line,
                  sep);
        break;
      }
    }
  }
}

// bufsz is the allocate size of the buffer
// strsz is the number of bytes in the buffer currently used
// str is the buffer
// append is a 0-terminated string to append
static void append_to_string(size_t* bufszArg, size_t* strszArg, char** strArg,
                             const char* append) {
  size_t toadd = strlen(append);
  size_t bufsz = *bufszArg;
  size_t strsz = *strszArg;
  char* str = *strArg;

  // allocate/reallocate the buffer if necessary
  if (str == NULL) {
    bufsz = 128 + toadd;
    str = chpl_mem_alloc(bufsz, CHPL_RT_MD_IO_BUFFER, __LINE__, 0);
    strsz = 0;
  } else if (strsz + toadd + 1 > bufsz) {
    bufsz = 2*bufsz + strsz + toadd;
    str = chpl_mem_realloc(str, bufsz, CHPL_RT_MD_IO_BUFFER, __LINE__, 0);
  }
  strncpy(str + strsz, append, toadd);
  strsz += toadd;

  *bufszArg = bufsz;
  *strszArg = strsz;
  *strArg = str;
}


char* chpl_stack_unwind_to_string(char sep) {
  // This is just a prototype using libunwind
  unw_cursor_t cursor;
  unw_context_t uc;
  unw_word_t wordValue;
  char buffer[256];
  int buffersz = 0;
  unsigned int line;

  char sepstr[2] = {sep, '\0'};

  char* str = NULL;
  size_t bufsz = 0;
  size_t strsz= 0;

  line = 0;
  unw_getcontext(&uc);
  unw_init_local(&cursor, &uc);

  if(chpl_sizeSymTable > 0) {
    append_to_string(&bufsz, &strsz, &str, "Stacktrace");
    append_to_string(&bufsz, &strsz, &str, sepstr);
    append_to_string(&bufsz, &strsz, &str, sepstr);
  }

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
        unw_proc_info_t info;
        // Maybe we can get a more precise line number
        unw_get_proc_info(&cursor, &info);
        line = chpl_unwind_getLineNum((void *)(info.start_ip + wordValue));
        // We wasn't able to obtain the line number, let's use the procedure
        // line number
        if(line == 0)
          line = chpl_filenumSymTable[t+1];
#else
        line = chpl_filenumSymTable[t+1];
#endif

        buffersz = snprintf(buffer, sizeof(buffer), "%d", line);
        if (buffersz < 0)
          buffer[0] = '\0';

        append_to_string(&bufsz, &strsz, &str, chpl_funSymTable[t+1]);
        append_to_string(&bufsz, &strsz, &str, "() at ");
        append_to_string(&bufsz, &strsz, &str,
                         chpl_lookupFilename(chpl_filenumSymTable[t]));
        append_to_string(&bufsz, &strsz, &str, ":");
        append_to_string(&bufsz, &strsz, &str, buffer); // line number
        append_to_string(&bufsz, &strsz, &str, sepstr);

        break;
      }
    }
  }

  // add null terminator
  if (str != NULL) {
    str[strsz] = '\0';
  }

  return str;
}

#else

void chpl_stack_unwind(FILE* out, char sep) {
}

char* chpl_stack_unwind_to_string(char sep) {
  return NULL;
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
  chpl_stack_unwind(stderr, '\n');
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
  chpl_stack_unwind(stderr, '\n');
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
