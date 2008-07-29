#ifndef _chplcast_h_
#define _chplcast_h_

#include "chpltypes.h"
#include <stdint.h>


/* invalid is a flag indicating valid or not
   invalidChar is the character that was invalid */
int8_t _string_to_int8_t_precise(const char* str, int* invalid, char* invalidChar);
int16_t _string_to_int16_t_precise(const char* str, int* invalid, char* invalidChar);
int32_t _string_to_int32_t_precise(const char* str, int* invalid, char* invalidChar);
int64_t _string_to_int64_t_precise(const char* str, int* invalid, char* invalidChar);
uint8_t _string_to_uint8_t_precise(const char* str, int* invalid, char* invalidChar);
uint16_t _string_to_uint16_t_precise(const char* str, int* invalid, char* invalidChar);
uint32_t _string_to_uint32_t_precise(const char* str, int* invalid, char* invalidChar);
uint64_t _string_to_uint64_t_precise(const char* str, int* invalid, char* invalidChar);

/* string to every other primitive type */
int8_t _string_to_int8_t(const char* str, int lineno, const char* filename);
int16_t _string_to_int16_t(const char* str, int lineno, const char* filename);
int32_t _string_to_int32_t(const char* str, int lineno, const char* filename);
int64_t _string_to_int64_t(const char* str, int lineno, const char* filename);
uint8_t _string_to_uint8_t(const char* str, int lineno, const char* filename);
uint16_t _string_to_uint16_t(const char* str, int lineno, const char* filename);
uint32_t _string_to_uint32_t(const char* str, int lineno, const char* filename);
uint64_t _string_to_uint64_t(const char* str, int lineno, const char* filename);

chpl_bool _string_to_chpl_bool(const char* str, int lineno, const char* filename);

_real32 _string_to_real32_precise(const char* str, int* invalid, char* invalidCh);
_real64 _string_to_real64_precise(const char* str, int* invalid, char* invalidCh);
_imag32 _string_to_imag32_precise(const char* str, int* invalid, char* invalidCh);
_imag64 _string_to_imag64_precise(const char* str, int* invalid, char* invalidCh);
_complex64 _string_to_complex64_precise(const char* str, int* invalid, char* invalidCh);
_complex128 _string_to_complex128_precise(const char* str, int* invalid, char* invalidCh);

_real32 _string_to_real32(const char* str, int lineno, const char* filename);
_real64 _string_to_real64(const char* str, int lineno, const char* filename);
_imag32 _string_to_imag32(const char* str, int lineno, const char* filename);
_imag64 _string_to_imag64(const char* str, int lineno, const char* filename);
_complex64 _string_to_complex64(const char* str, int lineno, const char* filename);
_complex128 _string_to_complex128(const char* str, int lineno, const char* filename);


/* every other primitive type to string */
_string int8_t_to_string(int8_t x);
_string int16_t_to_string(int16_t x);
_string int32_t_to_string(int32_t x);
_string int64_t_to_string(int64_t x);
_string uint8_t_to_string(uint8_t x);
_string uint16_t_to_string(uint16_t x);
_string uint32_t_to_string(uint32_t x);
_string uint64_t_to_string(uint64_t x);
_string _real32_to_string(_real32 x);
_string _real64_to_string(_real64 x);
_string _imag32_to_string(_imag32 x);
_string _imag64_to_string(_imag64 x);
#define chpl_bool_to_string(x) ((x) ? "true" : "false")
#define chpl_bool8_to_string(x) chpl_bool_to_string(x)
#define chpl_bool16_to_string(x) chpl_bool_to_string(x)
#define chpl_bool32_to_string(x) chpl_bool_to_string(x)
#define chpl_bool64_to_string(x) chpl_bool_to_string(x)

#endif
