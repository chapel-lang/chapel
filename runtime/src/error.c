/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
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

// #define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#endif
#ifndef DEBUG
#define DEBUG_PRINT(x) do {} while (0)
#endif

#ifdef CHPL_UNWIND_NOT_LAUNCHER
// Necessary for instruct libunwind to use only the local unwind
#define UNW_LOCAL_ONLY
#include <libunwind.h>
#include <dlfcn.h>

#ifdef __linux__
// We create a pipe with addr2line and try to get a line number
static int chpl_unwind_refineGetLineNum(void *addr) {

  int rc;
  Dl_info info;
  intptr_t relativeAddr;
  char buf[2048];
  int i = 0;
  int line;
  char* p;
  FILE *f;
  ssize_t path_len;


  // We use a little shell script for avoiding the case in which
  // addr2line isn't present.
  // We try the following commands in order. llvm-addr2line is preferred
  // since it woks betters with LLVM/clang and also works with gnu
  // but, llvm-addr2line might not be present, in which case we try
  // llvm-symbolizer (llvm-addr2line is a symlink to llvm-symbolizer anyways).
  // https://llvm.org/docs/CommandGuide/llvm-addr2line.html
  // 1. CHPL_LLVM_BIN_DIR/llvm-addr2line
  // 2. CHPL_LLVM_BIN_DIR/llvm-symbolizer
  // 3. llvm-addr2line
  // 4. llvm-symbolizer
  // 5. addr2line

  const char* script =
    "my_addr2line() { "
    "addr2line_cmd=$1/llvm-addr2line; shift; addr2line_args= ; "
    "if ! command -v $addr2line_cmd >/dev/null 2>&1; then "
      "addr2line_cmd=$1/llvm-symbolizer; "
      "addr2line_args='--functions=none --no-demangle --output-style=GNU'; "
      "if ! command -v $addr2line_cmd >/dev/null 2>&1; then "
        "addr2line_cmd=llvm-addr2line; addr2line_args= ; "
        "if ! command -v $addr2line_cmd >/dev/null 2>&1; then "
          "addr2line_cmd=llvm-symbolizer; "
          "addr2line_args='--functions=none --no-demangle --output-style=GNU'; "
          "if ! command -v $addr2line_cmd >/dev/null 2>&1; then "
            "addr2line_cmd=addr2line; addr2line_args= ; "
            "if ! command -v $addr2line_cmd >/dev/null 2>&1; then "
              "addr2line_cmd= ; addr2line_args= ; "
            "fi; "
          "fi; "
        "fi; "
      "fi; "
    "fi; "
    "if [ -n \"$addr2line_cmd\" ]; then "
      "$addr2line_cmd $addr2line_args -e $@ ; "
    "fi } ; my_addr2line "
    ;
  // then CHPL_LLVM_BIN_DIR
  // then space
  // then the path
  // then space
  // then the address

  int scriptLen = strlen(script);
  int llvmBinDirLen = strlen(CHPL_LLVM_BIN_DIR);

  // Start the buffer out with the script + CHPL_LLVM_BIN_DIR + space
  assert(sizeof(buf) > scriptLen);
  memcpy(buf, script, scriptLen);
  if (llvmBinDirLen+1 >= sizeof(buf)-scriptLen)
    return 0; // not enough room in buffer - give up
  memcpy(&buf[scriptLen], CHPL_LLVM_BIN_DIR, llvmBinDirLen);
  memcpy(&buf[scriptLen+llvmBinDirLen], " ", 1);
  i = scriptLen + llvmBinDirLen + 1;

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
      return 0; // truncation occurred - give up.
    if (path_len == -1)
      return 0; // readlink returned error - give up.
  }
  i += path_len;

  rc = snprintf(&buf[i], sizeof(buf)-i,
                " %p", (void*)relativeAddr);
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
#elif defined __APPLE__
// invoke atos to get line number information
static int chpl_unwind_refineGetLineNum(void *addr) {
  char buf[2048];
  int rc;

  // atos -o EXECUTABLE_PATH -offset REL_ADDRESS
  // Compute the object containing the address
  Dl_info info;
  rc = dladdr(addr, &info);
  if (rc == 0)
    return 0; // dladdr failed.

  // Compute the relative address within the object
  intptr_t relativeAddr = (intptr_t)addr - (intptr_t)info.dli_fbase;

  // Compute the path to the file containing the object
  if (info.dli_fname != NULL && info.dli_fname[0] != '\0') {
    // use the path from dladdr to construct atos cmd
    rc = snprintf(buf, sizeof(buf),
                  "atos -o %s --fullPath -offset %p",
                  info.dli_fname, (void*)relativeAddr);
    if (rc+1 >= sizeof(buf))
      return 0; // command too long for buffer - give up
  } else {
    // we failed to get the file name
    // TODO: we could maybe invoke atos with getpid?
    return 0;
  }

  FILE* f = popen(buf, "r");
  if (f == NULL)
    return 0; // popen failed - give up

  char* p = fgets(buf, sizeof(buf), f);
  if (p == NULL) {
    // couldn't read from the pipe - close and give up
    pclose(f);
    return 0;
  }
  pclose(f);
  // format is '<FN_NAME> (in <REL_EXEC_NAME>) (<FILENAME>:<LINENUME>)
  // search from the end of the string backwards for ':'
  p = buf + strlen(buf) - 1;
  while (p > buf && *p != ':') { p--; }
  if (p == buf)
    return 0; // didn't find ':' - give up
  p++; // move past ':'

  int line = atoi(p);
  return line;
}
#else
static int chpl_unwind_refineGetLineNum(void *addr) {
  // Not implemented on this platform
  return 0;
}
#endif


static unsigned int chpl_unwind_getLineNum(unw_cursor_t* cursor,
                                           unw_word_t wordValue,
                                           int tableIdx) {
  // use the procedure line number
  unsigned int line = chpl_filenumSymTable[tableIdx + 1];

  // try and use dladdr to get a more precise line number
  unw_proc_info_t info;
  // Maybe we can get a more precise line number
  unw_get_proc_info(cursor, &info);
  unsigned int lineTmp = chpl_unwind_refineGetLineNum((void *)(info.start_ip + wordValue));
  if (lineTmp != 0)
    line = lineTmp;

  return line;
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

enum chpl_stack_unwind_mode {
  CHPL_STACK_UNWIND_MODE_FILE,
  CHPL_STACK_UNWIND_MODE_STRING
};


// Helper function for chpl_stack_unwind and chpl_stack_unwind_to_string
// mode indicates whether we are writing to a FILE* or to a string
// out is FILE* or char**, depending on the mode
static void chpl_stack_unwind_helper(enum chpl_stack_unwind_mode mode, char sep, void* out) {

  // This is just a prototype using libunwind
  unw_cursor_t cursor;
  unw_context_t uc;
  unw_word_t wordValue;
  char buffer[256];

  if (unw_getcontext(&uc) != 0) {
    DEBUG_PRINT(("unw_getcontext failed\n"));
    return;
  }
  if (unw_init_local(&cursor, &uc) != 0) {
    DEBUG_PRINT(("unw_init_local failed\n"));
    return;
  }

  // only used with CHPL_STACK_UNWIND_MODE_STRING
  size_t bufsz = 0;
  size_t strsz = 0;
  char** strPtr = (char**)out;
  char sepstr[2] = {sep, '\0'};

  if (chpl_sizeSymTable > 0) {
    switch (mode) {
      case CHPL_STACK_UNWIND_MODE_FILE:
        fprintf((FILE*)out,"Stacktrace%c%c", sep, sep);
      break;
      case CHPL_STACK_UNWIND_MODE_STRING: {
        append_to_string(&bufsz, &strsz, strPtr, "Stacktrace");
        append_to_string(&bufsz, &strsz, strPtr, sepstr);
        append_to_string(&bufsz, &strsz, strPtr, sepstr);
      }
      break;
    }
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
    for (int t = 0; t < chpl_sizeSymTable; t+=2 ) {
      if (!strcmp(chpl_funSymTable[t], buffer)) {
        unsigned int line = chpl_unwind_getLineNum(&cursor, wordValue, t);
        switch (mode) {
          case CHPL_STACK_UNWIND_MODE_FILE:
            fprintf((FILE*)out, "%s() at %s:%d%c",
                     chpl_funSymTable[t+1],
                     chpl_lookupFilename(chpl_filenumSymTable[t]),
                     line,
                     sep);
          break;
          case CHPL_STACK_UNWIND_MODE_STRING: {
            int buffersz = snprintf(buffer, sizeof(buffer), "%d", line);
            if (buffersz < 0)
              buffer[0] = '\0';

            append_to_string(&bufsz, &strsz, strPtr, chpl_funSymTable[t+1]);
            append_to_string(&bufsz, &strsz, strPtr, "() at ");
            append_to_string(&bufsz, &strsz, strPtr,
                            chpl_lookupFilename(chpl_filenumSymTable[t]));
            append_to_string(&bufsz, &strsz, strPtr, ":");
            append_to_string(&bufsz, &strsz, strPtr, buffer); // line number
            append_to_string(&bufsz, &strsz, strPtr, sepstr);
          }
          break;
        }
      }
    }
  }

  if (mode == CHPL_STACK_UNWIND_MODE_STRING) {
    // add null terminator
    if (*strPtr != NULL) {
      (*strPtr)[strsz] = '\0';
    }
  }
}


void chpl_stack_unwind(FILE* out, char sep) {
  const char* chpl_rt_unwind = chpl_env_rt_get("UNWIND", NULL);
  chpl_bool should_print = chpl_env_str_to_bool("UNWIND", chpl_rt_unwind, true);
  chpl_bool user_set = chpl_rt_unwind != NULL;
  if (!should_print) {
    return;
  }
  chpl_stack_unwind_helper(CHPL_STACK_UNWIND_MODE_FILE, sep, out);
  if (!user_set && strcmp(CHPL_UNWIND, "none") != 0) {
    fprintf(out, "%cDisable full stacktrace by setting 'CHPL_RT_UNWIND=0'%c", sep, sep);
  }
}

char* chpl_stack_unwind_to_string(char sep) {
  char* str = NULL;
  chpl_stack_unwind_helper(CHPL_STACK_UNWIND_MODE_STRING, sep, (void*)&str);
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
static chpl_atomic_bool thisLocaleAlreadyExiting;
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
