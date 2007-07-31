#ifndef _chplcast_h_
#define _chplcast_h_

#include <stdlib.h>         // needed for strtoull
#include "chpltypes.h"

/* string to every other primitive type */
#define _string_to_int8(s) ((_int8)strtoull((s), NULL, 10))
#define _string_to_int16(s) ((_int16)strtoull((s), NULL, 10))
#define _string_to_int32(s) ((_int32)strtoull((s), NULL, 10))
#define _string_to_int64(s) ((_int64)strtoull((s), NULL, 10))
#define _string_to_uint8(s) ((_uint8)strtoull((s), NULL, 10))
#define _string_to_uint16(s) ((_uint16)strtoull((s), NULL, 10))
#define _string_to_uint32(s) ((_uint32)strtoull((s), NULL, 10))
#define _string_to_uint64(s) ((_uint64)strtoull((s), NULL, 10))
#define _string_to_real32(s) ((_real32)atof(s))
#define _string_to_real64(s) ((_real64)atof(s))
#define _string_to_real128(s) ((_real128)atof(s))
#define _string_to_imag32(s) ((_imag32)atof(s))
#define _string_to_imag64(s) ((_imag64)atof(s))
#define _string_to_imag128(s) ((_imag128)atof(s))
_complex64 _string_to_complex64(_string s);
_complex128 _string_to_complex128(_string s);
_complex256 _string_to_complex256(_string s);
#define _string_to_bool(s) (string_equal(s, "true"))

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
_string _complex64_to_string(_complex64 x);
_string _complex128_to_string(_complex128 x);
_string _complex256_to_string(_complex256 x);
#define _bool_to_string(x) ((x) ? "true" : "false")

#endif
