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

/*
 * chpl-string-support.c
 *
 * This file provides utility functions for C strings (char *) and is
 * used by the runtime, the Chapel generated code, as well as the
 * launcher.
 *
 * Note that the naming here is not ideal and is a byproduct of when
 * we used Chapel strings (chpl_string) and C strings (c_string)
 * interchangeably.  When the Chapel string implementation is actually
 * different than the runtime type c_string, we should go back and
 * considering naming things more consistently.
 *
 */
#include <stdarg.h>
#include "chplrt.h"
#include "sys_basic.h"
#include "chpl-mem.h"
#include "chpl-mem-desc.h"
#include "chpltypes.h"
#include "error.h"

// Uses the system allocator.  Should not be used to create user-visible data
// (error messages are OK).
char* chpl_glom_strings(int numstrings, ...) {
  va_list ap;
  int i, len;
  char* str;

  va_start(ap, numstrings);
  len = 0;
  for (i=0; i<numstrings; i++)
    len += strlen(va_arg(ap, const char*));
  va_end(ap);

  str = (char*)chpl_mem_allocMany(len+1, sizeof(char),
                                  CHPL_RT_MD_GLOM_STRINGS_DATA, 0, 0);

  va_start(ap, numstrings);
  str[0] = '\0';
  for (i=0; i<numstrings; i++)
    strcat(str, va_arg(ap, char*));
  va_end(ap);

  return str;
}


c_string chpl_format(c_string format, ...) {
  va_list ap;
  char z[128];

  va_start(ap, format);
  if (vsnprintf(z, sizeof(z), format, ap) >= sizeof(z))
    chpl_error("overflow encountered in format", 0, 0);
  va_end(ap);
  return string_copy(z, 0, 0);
}


c_string
string_copy(c_string x, int32_t lineno, int32_t filename)
{
  char *z;

  // If the input string is null, just return null.
  if (x == NULL)
    return NULL;

  z = (char*)chpl_mem_allocMany(1, strlen(x)+1, CHPL_RT_MD_STR_COPY_DATA,
                                lineno, filename);
  return strcpy(z, x);
}

// string_concat always returns a newly-allocated c_string (or NULL).
c_string
string_concat(c_string x, c_string y, int32_t lineno, int32_t filename) {
  char* z;
  size_t xlen;
  size_t ylen;

  if (x == NULL)
    return string_copy(y, lineno, filename);
  if (y == NULL)
    return string_copy(x, lineno, filename);

  xlen = strlen(x);
  ylen = strlen(y);

  z = (char*)chpl_mem_allocMany(1, xlen + ylen + 1,
                                CHPL_RT_MD_STR_CONCAT_DATA,
                                lineno, filename);

  // memcpy can be more efficient than the str??? functions because it does not
  // look for terminating NUL characters.  We are guaranteed that the source
  // and destination strings do not overlap so we don't need the (very slight)
  // extra complexity of memmove.
  memcpy(z, x, xlen);
  // Using "ylen+1" copies the terminating NUL from the second string.
  memcpy(z + xlen, y, ylen+1);

  return z;
}

// Returns the index of the first occurrence of a substring within a string, or
// 0 if the substring is not in the string.
int string_index_of(c_string haystack, c_string needle) {
  c_string substring = strstr(haystack, needle);
  return substring ? (int) (substring-haystack)+1 : 0;
}

// Returns a newly-allocated string containing (a copy of) the bytes selected
// from the original string.
// It is up to the caller to make sure low and high are within the string
// bounds and that stride is not 0.
c_string
string_select(c_string x, int low, int high, int stride, int32_t lineno, int32_t filename) {
  char* result = NULL;
  char* dst = NULL;
  int size;
  c_string src;

  if (low  < 1) low = 1;
  if (high < low) return NULL;

  size = high - low + 1;
  result = chpl_mem_allocMany(1, size + 1, CHPL_RT_MD_STR_SELECT_DATA,
                              lineno, filename);
  src = stride > 0 ? x + low - 1 : x + high - 1;
  dst = result;
  if (stride == 1) {
    memcpy(result, src, size);
    dst = result + size;
  } else if (stride > 0) {
    while (src - x <= high - 1) {
      *dst++ = *src;
      src += stride;
    }
  } else {
    while (src - x >= low - 1) {
      *dst++ = *src;
      src += stride;
    }
  }

  *dst = '\0';
  return result;
}

// Returns a string containing the character at the given index of the input
// string, or an empty string if the index is out of bounds.
c_string
string_index(c_string x, int i, int32_t lineno, int32_t filename) {
  char* buffer;
  if (i-1 < 0 || i-1 >= string_length_bytes(x))
  {
    return NULL;
  }
  buffer = chpl_mem_allocMany(1, 2, CHPL_RT_MD_STR_COPY_DATA,
                              lineno, filename);
  sprintf(buffer, "%c", x[i-1]);
  return buffer;
}


chpl_bool
string_contains(c_string x, c_string y) {
  if (strstr(x, y))
    return true;
  else
    return false;
}


