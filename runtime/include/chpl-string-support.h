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

#ifndef _chpl_string_support_h_
#define _chpl_string_support_h_
/*
 * chpl-string-support.h
 *
 * This file provides utility functions for C strings (char *) and is
 * used by the runtime, the Chapel generated code, as well as the
 * launcher.
 *
 * See comments at the top of chpl-string-support.c for more info.
 *
 */

// A low-level C String, suitable for use with OS support functions &c.
// c_string behaves like a reference: The caller should not free a returned
// c_string, and should make a copy to preserve its value e.g. in a structure
// or global variable.
// The returned value is not guaranteed to remain the same through multiple
// calls.  Also, the returned value may change due to the activity of other
// threads.  To ensure thread-safety, use only functions that return a
// c_string_copy, or that guarantee thread-safety.
typedef const char* c_string;

// This type is the same as c_string except when used as a return value type it
// guarantees that the returned string is newly heap-allocated.  The caller of
// a routine returning a c_string_copy must free the returned copy or pass
// ownership to another program entity (caller, callee or data structure).
// It is permissible to pass a c_string_copy as an argument where a c_string is
// expected.
typedef const char* c_string_copy;

#include "chpltypes.h"
#include <string.h>

static ___always_inline
int8_t ascii(c_string s) {
  return (int8_t) *s;
}

static ___always_inline
int64_t string_length(c_string x) {
  if (x == NULL)
    return 0;
  return strlen(x);
}

static ___always_inline
int32_t string_compare(c_string x, c_string y) {
  if (x == y) return 0;
  if (x == NULL) return -1;
  if (y == NULL) return 1;
  return (int32_t)strcmp(x, y);
}

//
// stopgap formatting
//
c_string_copy chpl_format(c_string format, ...)
  __attribute__((format(printf, 1, 2)));

// Uses the system allocator.
char* chpl_glom_strings(int numstrings, ...);

chpl_bool string_contains(c_string x, c_string y);
c_string_copy string_copy(c_string x, int32_t lineno, c_string filename);
c_string_copy string_concat(c_string x, c_string y, int32_t lineno, c_string filename);
int string_index_of(c_string x, c_string y);
c_string_copy string_index(c_string x, int i, int32_t lineno, c_string filename);
// TODO: A separate unstrided version could return a c_string instead.
c_string_copy string_select(c_string x, int low, int high, int stride, int32_t lineno, c_string filename);

#endif
