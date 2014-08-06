#ifndef _chpl_string_support_h_
#define _chpl_string_support_h_

typedef const char* c_string;

#include <string.h>

static ___always_inline
int8_t ascii(c_string s) {
  return (int8_t) *s;
}

static ___always_inline
int64_t string_length(c_string x) {
  return strlen(x);
}

static ___always_inline
int32_t string_compare(c_string x, c_string y) {
  return (int32_t)strcmp(x, y);
}

//
// stopgap formatting
//
c_string chpl_format(c_string format, ...)
  __attribute__((format(printf, 1, 2)));

// Uses the system allocator.
char* chpl_glom_strings(int numstrings, ...);

chpl_bool string_contains(c_string x, c_string y);
c_string string_copy(c_string x, int32_t lineno, c_string filename);
c_string string_concat(c_string x, c_string y, int32_t lineno, c_string filename);
int string_index_of(c_string x, c_string y);
c_string string_index(c_string x, int i, int32_t lineno, c_string filename);
c_string string_select(c_string x, int low, int high, int stride, int32_t lineno, c_string filename);

#endif
