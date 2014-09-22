/*
 * Copyright 2004-2014 Cray Inc.
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

// Chapel string support functions
c_string stringMove(c_string dest, c_string src, int64_t len,
                    int32_t lineno, c_string filename);
c_string remoteStringCopy(c_nodeid_t src_locale,
                          c_string src_addr, int64_t src_len,
                          int32_t lineno, c_string filename);

#endif
