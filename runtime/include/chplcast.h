/*
 * Copyright 2004-2020 Cray Inc.
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
int8_t c_string_to_int8_t(c_string str, chpl_bool* err, int lineno, int32_t filename);
int16_t c_string_to_int16_t(c_string str, chpl_bool* err, int lineno, int32_t filename);
int32_t c_string_to_int32_t(c_string str, chpl_bool* err, int lineno, int32_t filename);
int64_t c_string_to_int64_t(c_string str, chpl_bool* err, int lineno, int32_t filename);
uint8_t c_string_to_uint8_t(c_string str, chpl_bool* err, int lineno, int32_t filename);
uint16_t c_string_to_uint16_t(c_string str, chpl_bool* err, int lineno, int32_t filename);
uint32_t c_string_to_uint32_t(c_string str, chpl_bool* err, int lineno, int32_t filename);
uint64_t c_string_to_uint64_t(c_string str, chpl_bool* err, int lineno, int32_t filename);

chpl_bool c_string_to_chpl_bool(c_string str, chpl_bool* err, int lineno, int32_t filename);

_real32 c_string_to_real32_precise(c_string str, int* invalid, char* invalidCh);
_real64 c_string_to_real64_precise(c_string str, int* invalid, char* invalidCh);
_imag32 c_string_to_imag32_precise(c_string str, int* invalid, char* invalidCh);
_imag64 c_string_to_imag64_precise(c_string str, int* invalid, char* invalidCh);
#ifndef __cplusplus
_complex64 c_string_to_complex64_precise(c_string str, int* invalid, char* invalidCh);
_complex128 c_string_to_complex128_precise(c_string str, int* invalid, char* invalidCh);
#endif

_real32 c_string_to_real32(c_string str, chpl_bool* err, int lineno, int32_t filename);
_real64 c_string_to_real64(c_string str, chpl_bool* err, int lineno, int32_t filename);
_imag32 c_string_to_imag32(c_string str, chpl_bool* err, int lineno, int32_t filename);
_imag64 c_string_to_imag64(c_string str, chpl_bool* err, int lineno, int32_t filename);
#ifndef __cplusplus
_complex64 c_string_to_complex64(c_string str, chpl_bool* err, int lineno, int32_t filename);
_complex128 c_string_to_complex128(c_string str, chpl_bool* err, int lineno, int32_t filename);
#endif


/* every other primitive type to string */
c_string integral_to_c_string(int64_t x, uint32_t size, chpl_bool isSigned, chpl_bool* err);
c_string real_to_c_string(_real64 x, chpl_bool isImag);

#include "chpl-string.h"
#endif // _chplcast_h_
