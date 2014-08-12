#ifndef _chpl_string_h_
#define _chpl_string_h_

#include "chpl-string-support.h"

typedef const char* chpl_string;

#ifdef _stdchpl_H_
/*** only needed for generated code ***/
chpl_string defaultStringValue="";
#endif

struct chpl_chpl____wide_chpl_string_s;

chpl_string chpl_wide_string_copy(struct chpl_chpl____wide_chpl_string_s* x, int32_t lineno, c_string filename);
void chpl_string_widen(struct chpl_chpl____wide_chpl_string_s* x, chpl_string from, int32_t lineno, c_string filename);
void chpl_comm_wide_get_string(chpl_string* local, struct chpl_chpl____wide_chpl_string_s* x, int32_t tid, int32_t lineno, c_string filename);

// implement string_from_c_string and c_string_from_string primitives.
void string_from_c_string(chpl_string *ret, c_string str, int haslen, int64_t len, int32_t lineno, c_string filename);
void wide_string_from_c_string(struct chpl_chpl____wide_chpl_string_s *ret, c_string str, int haslen, int64_t len, int32_t lineno, c_string filename);
void c_string_from_string(c_string* ret, chpl_string* str, int32_t lineno, c_string filename);
void c_string_from_wide_string(c_string* ret, struct chpl_chpl____wide_chpl_string_s* str, int32_t lineno, c_string filename);

#endif
