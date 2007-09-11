#ifndef _chplcast_h_
#define _chplcast_h_

#include "chpltypes.h"


_int8 _string_to_int8_precise(const char* str, int* valid);
_int16 _string_to_int16_precise(const char* str, int* valid);
_int32 _string_to_int32_precise(const char* str, int* valid);
_int64 _string_to_int64_precise(const char* str, int* valid);
_uint8 _string_to_uint8_precise(const char* str, int* valid);
_uint16 _string_to_uint16_precise(const char* str, int* valid);
_uint32 _string_to_uint32_precise(const char* str, int* valid);
_uint64 _string_to_uint64_precise(const char* str, int* valid);

/* string to every other primitive type */
_int8 _string_to_int8(const char* str);
_int16 _string_to_int16(const char* str);
_int32 _string_to_int32(const char* str);
_int64 _string_to_int64(const char* str);
_uint8 _string_to_uint8(const char* str);
_uint16 _string_to_uint16(const char* str);
_uint32 _string_to_uint32(const char* str);
_uint64 _string_to_uint64(const char* str);

#define _string_to_real32(s) ((_real32)atof(s))
#define _string_to_real64(s) ((_real64)atof(s))
#define _string_to_real128(s) ((_real128)atof(s))
#define _string_to_imag32(s) ((_imag32)atof(s))
#define _string_to_imag64(s) ((_imag64)atof(s))
#define _string_to_imag128(s) ((_imag128)atof(s))
#define _string_to_bool(s) (string_equal(s, "true"))
_string _string_get_imag_part(_string s);


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
