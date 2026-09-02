/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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

#include "chplrt.h"
#include "chpl-linefile-support.h"

#include "chpl-error.h"
#include "chplexit.h"
#include "chpl-mem.h"
#include "chpl-env.h"
#include "chpl-exec.h"
#include "chpl-unwind.h"

#include <inttypes.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef LAUNCHER
#include "chpl-atomics.h"
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

  chpl_stack_unwind(stderr, '\n');

  chpl_exit_any(1);
}


__attribute__ ((format (printf, 6, 0)))
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


__attribute__ ((format (printf, 5, 0)))
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

  chpl_stack_unwind(stderr, '\n');

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
