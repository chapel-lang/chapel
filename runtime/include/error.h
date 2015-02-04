/*
 * Copyright 2004-2015 Cray Inc.
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
#include <stdint.h>

extern int verbosity;

#ifndef CHPL_RT_UNIT_TEST
void chpl_warning(const char* message, int32_t lineno, c_string filename);
void chpl_error(const char* message, int32_t lineno, c_string filename);
void chpl_internal_error(const char* message);
#else
#define chpl_warning(message, lineno, filename) \
    do {                                        \
      fflush(stdout);                           \
      fprintf(stderr, "%s:%" PRId32 ": warning: %s\n", filename, lineno, message); \
    } while (0)

#define chpl_error(message, lineno, filename) \
    do {                                      \
      fflush(stdout);                         \
      fprintf(stderr, "%s:%" PRId32 ": error: %s\n", filename, lineno, message); \
      exit(1);                                \
    } while (0)

#define chpl_internal_error(message) \
    do {                             \
      fflush(stdout);                \
      fprintf(stderr, "internal error: %s\n", message); \
      exit(1);                       \
    } while (0)
#endif
void chpl_msg(int verbose_level, const char* fmt, ...)
  __attribute__((format(printf, 2, 3)));

#ifndef LAUNCHER
void chpl_error_init(void);
#endif

#endif
