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

#ifndef _error_H_
#define _error_H_

#include "chpltypes.h"

#include <stdarg.h>
#include <stdint.h>

extern int verbosity;

// These functions eventually call out to chpl_exit_any, or try to perform a
// filename index -> filename lookup, neither of which can be performed while
// the runtime is being unit tested
#ifndef CHPL_RT_UNIT_TEST
void chpl_warning(const char* message, int32_t lineno, int32_t filenameIdx);
void chpl_warning_explicit(const char *message, int32_t lineno,
                           const char *filename);
void chpl_error_preformatted(const char* message);
void chpl_error(const char* message, int32_t lineno, int32_t filenameIdx);
void chpl_error_explicit(const char *message, int32_t lineno,
                         const char *filename);
void chpl_internal_error(const char* message);
void chpl_internal_error_v(const char *restrict format, ...)
       __attribute__((format(printf, 1, 2)));
#else
// Filename is now an int32_t index into a table that we are not going to have
// while the runtime is in unit test mode, just print out the message instead
#define chpl_warning(message, lineno, filename)                                \
  do {                                                                         \
    fflush(stdout);                                                            \
    fprintf(stderr, "warning: %s\n", message);                                 \
  } while (0)

#define chpl_warning_explicit(message, lineno, filename)                       \
  do {                                                                         \
    fflush(stdout);                                                            \
    fprintf(stderr, "%s:%" PRId32 ": warning: %s\n", filename, lineno,         \
            message);                                                          \
  } while (0)

#define chpl_error(message, lineno, filename)                                  \
  do {                                                                         \
    fflush(stdout);                                                            \
    fprintf(stderr, "error: %s\n", message);                                   \
    exit(1);                                                                   \
  } while (0)

#define chpl_error_explicit(message, lineno, filename)                         \
  do {                                                                         \
    fflush(stdout);                                                            \
    fprintf(stderr, "%s:%" PRId32 ": error: %s\n", filename, lineno, message); \
    exit(1);                                                                   \
  } while (0)

#define chpl_internal_error(message) chpl_internal_error_v("%s", message)

static inline
void chpl_internal_error_v(const char *restrict, ...)
    __attribute__((format(printf, 1, 2)));

static inline
void chpl_internal_error_v(const char *restrict format, ...) {
  fflush(stdout);
  fprintf(stderr, "internal error: ");

  va_list ap;
  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);

  exit(1);
}
#endif

void chpl_msg(int verbose_level, const char* fmt, ...)
  __attribute__((format(printf, 2, 3)));

#ifndef LAUNCHER
void chpl_error_init(void);
#endif

#endif
