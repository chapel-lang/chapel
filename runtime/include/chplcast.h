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

#ifndef _chplcast_h_
#define _chplcast_h_

#include "chpltypes.h"
#include <stdint.h>


/* invalid is a flag indicating valid or not
   invalidChar is the character that was invalid */
int8_t c_string_to_int8_t_precise(c_string str, int* invalid, char* invalidChar);
int16_t c_string_to_int16_t_precise(c_string str, int* invalid, char* invalidChar);
int32_t c_string_to_int32_t_precise(c_string str, int* invalid, char* invalidChar);
int64_t c_string_to_int64_t_precise(c_string str, int* invalid, char* invalidChar);
uint8_t c_string_to_uint8_t_precise(c_string str, int* invalid, char* invalidChar);
uint16_t c_string_to_uint16_t_precise(c_string str, int* invalid, char* invalidChar);
uint32_t c_string_to_uint32_t_precise(c_string str, int* invalid, char* invalidChar);
uint64_t c_string_to_uint64_t_precise(c_string str, int* invalid, char* invalidChar);

/* string to every other primitive type */
int8_t c_string_to_int8_t(c_string str, int lineno, c_string filename);
int16_t c_string_to_int16_t(c_string str, int lineno, c_string filename);
int32_t c_string_to_int32_t(c_string str, int lineno, c_string filename);
int64_t c_string_to_int64_t(c_string str, int lineno, c_string filename);
uint8_t c_string_to_uint8_t(c_string str, int lineno, c_string filename);
uint16_t c_string_to_uint16_t(c_string str, int lineno, c_string filename);
uint32_t c_string_to_uint32_t(c_string str, int lineno, c_string filename);
uint64_t c_string_to_uint64_t(c_string str, int lineno, c_string filename);

chpl_bool c_string_to_chpl_bool(c_string str, int lineno, c_string filename);

_real32 c_string_to_real32_precise(c_string str, int* invalid, char* invalidCh);
_real64 c_string_to_real64_precise(c_string str, int* invalid, char* invalidCh);
_imag32 c_string_to_imag32_precise(c_string str, int* invalid, char* invalidCh);
_imag64 c_string_to_imag64_precise(c_string str, int* invalid, char* invalidCh);
_complex64 c_string_to_complex64_precise(c_string str, int* invalid, char* invalidCh);
_complex128 c_string_to_complex128_precise(c_string str, int* invalid, char* invalidCh);

_real32 c_string_to_real32(c_string str, int lineno, c_string filename);
_real64 c_string_to_real64(c_string str, int lineno, c_string filename);
_imag32 c_string_to_imag32(c_string str, int lineno, c_string filename);
_imag64 c_string_to_imag64(c_string str, int lineno, c_string filename);
_complex64 c_string_to_complex64(c_string str, int lineno, c_string filename);
_complex128 c_string_to_complex128(c_string str, int lineno, c_string filename);


/* every other primitive type to string */
c_string_copy integral_to_c_string_copy(int64_t x, uint32_t size, chpl_bool isSigned);
c_string_copy real_to_c_string_copy(_real64 x, chpl_bool isImag);

// Since Chapel's type system treats c_string and c_string_copy as distinct
// types, we could rely on the Chapel compiler doing the right thing if these
// routines are used as externs in module code.
// TODO: Currently, they are called directly in codegen.  This is dangerous
// because the C compiler treats the two types identically.
// However, the distinction between c_string and c_string_copy should be
// respected, so we can free dynamically-allocated C strings and avoid freeing
// statically-allocated ones.  The easiest approach is probably to call these
// (and other type##_to_c_string functions) from module code.
// TODO: Can we use the pattern above, to reduce the number of interfaces required?
static ___always_inline
c_string chpl_bool_to_c_string(chpl_bool x) {
  return x ? "true" : "false";
}
static ___always_inline
c_string chpl_bool8_to_c_string(chpl_bool8 x) {
  return x ? "true" : "false";
}
static ___always_inline
c_string chpl_bool16_to_c_string(chpl_bool16 x) {
  return x ? "true" : "false";
}
static ___always_inline
c_string chpl_bool32_to_c_string(chpl_bool32 x) {
  return x ? "true" : "false";
}
static ___always_inline
c_string chpl_bool64_to_c_string(chpl_bool64 x) {
  return x ? "true" : "false";
}

#include "chpl-string.h"

// Note that this routine returns a reference to the character buffer in the
// underlying chpl_string, so the chpl_string must last longer than the
// returned c_string or the caller must make a copy of the result.  The caller
// should not free the returned string.
static ___always_inline
c_string chpl_string_to_c_string(chpl_string s, int lineno, c_string filename) {
  c_string ret;
  c_string_from_string(&ret, &s, lineno, filename);
  return ret; // leaky? No. (see above)
}

// This routine returns a chpl_string containing a verbatim copy of the
// passed-in C string.  The (chpl_)string destructor will take care of
// deallocating that memory.
static ___always_inline
chpl_string c_string_to_chpl_string(c_string s, int lineno, c_string filename) {
  chpl_string ret;
  string_from_c_string(&ret, s, 0, -1, lineno, filename);
  return ret; // leaky? No. (see above)
}

#endif // _chplcast_h_
