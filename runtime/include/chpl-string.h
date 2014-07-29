#ifndef _chpl_string_h_
#define _chpl_string_h_

typedef const char* c_string;
typedef const char* chpl_string;

#ifdef _stdchpl_H_
/*** only needed for generated code ***/
chpl_string defaultStringValue="";
#endif

static ___always_inline
int8_t ascii(c_string s) {
  return (int8_t) *s;
}

struct chpl_chpl____wide_chpl_string_s;

//
// stopgap formatting
//
c_string chpl_format(c_string format, ...)
  __attribute__((format(printf, 1, 2)));

// Uses the system allocator.
char* chpl_glom_strings(int numstrings, ...);

chpl_bool string_contains(c_string x, c_string y);
c_string string_copy(c_string x, int32_t lineno, c_string filename);
chpl_string chpl_wide_string_copy(struct chpl_chpl____wide_chpl_string_s* x, int32_t lineno, c_string filename);
void chpl_string_widen(struct chpl_chpl____wide_chpl_string_s* x, chpl_string from, int32_t lineno, c_string filename);
void chpl_comm_wide_get_string(chpl_string* local, struct chpl_chpl____wide_chpl_string_s* x, int32_t tid, int32_t lineno, c_string filename);
c_string string_concat(c_string x, c_string y, int32_t lineno, c_string filename);
int string_index_of(c_string x, c_string y);
c_string string_index(c_string x, int i, int32_t lineno, c_string filename);
c_string string_select(c_string x, int low, int high, int stride, int32_t lineno, c_string filename);
int32_t string_compare(c_string x, c_string y);
int64_t string_length(c_string x);

// implement string_from_c_string and c_string_from_string primitives.
void string_from_c_string(chpl_string *ret, c_string str, int haslen, int64_t len, int32_t lineno, c_string filename);
void wide_string_from_c_string(struct chpl_chpl____wide_chpl_string_s *ret, c_string str, int haslen, int64_t len, int32_t lineno, c_string filename);
void c_string_from_string(c_string* ret, chpl_string* str, int32_t lineno, c_string filename);
void c_string_from_wide_string(c_string* ret, struct chpl_chpl____wide_chpl_string_s* str, int32_t lineno, c_string filename);

#endif
