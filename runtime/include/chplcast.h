#ifndef _chplcast_h_
#define _chplcast_h_

#include "chpltypes.h"


/* invalid is a flag indicating valid or not
   invalidChar is the character that was invalid */
_int8 _string_to_int8_precise(const char* str, int* invalid, char* invalidChar);
_int16 _string_to_int16_precise(const char* str, int* invalid, char* invalidChar);
_int32 _string_to_int32_precise(const char* str, int* invalid, char* invalidChar);
_int64 _string_to_int64_precise(const char* str, int* invalid, char* invalidChar);
_uint8 _string_to_uint8_precise(const char* str, int* invalid, char* invalidChar);
_uint16 _string_to_uint16_precise(const char* str, int* invalid, char* invalidChar);
_uint32 _string_to_uint32_precise(const char* str, int* invalid, char* invalidChar);
_uint64 _string_to_uint64_precise(const char* str, int* invalid, char* invalidChar);

/* string to every other primitive type */
_int8 _string_to_int8(const char* str, int lineno, const char* filename);
_int16 _string_to_int16(const char* str, int lineno, const char* filename);
_int32 _string_to_int32(const char* str, int lineno, const char* filename);
_int64 _string_to_int64(const char* str, int lineno, const char* filename);
_uint8 _string_to_uint8(const char* str, int lineno, const char* filename);
_uint16 _string_to_uint16(const char* str, int lineno, const char* filename);
_uint32 _string_to_uint32(const char* str, int lineno, const char* filename);
_uint64 _string_to_uint64(const char* str, int lineno, const char* filename);

_bool _string_to_bool(const char* str, int lineno, const char* filename);

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
_string _int8_to_string(_int8 x);
_string _int16_to_string(_int16 x);
_string _int32_to_string(_int32 x);
_string _int64_to_string(_int64 x);
_string _uint8_to_string(_uint8 x);
_string _uint16_to_string(_uint16 x);
_string _uint32_to_string(_uint32 x);
_string _uint64_to_string(_uint64 x);
_string _real32_to_string(_real32 x);
_string _real64_to_string(_real64 x);
_string _real128_to_string(_real128 x);
_string _imag32_to_string(_imag32 x);
_string _imag64_to_string(_imag64 x);
_string _imag128_to_string(_imag128 x);
#define _bool_to_string(x) ((x) ? "true" : "false")

#endif
