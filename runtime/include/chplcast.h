#ifndef _chplcast_h_
#define _chplcast_h_

#define _string_to_int8(s) ((_int8)atoi(s))
#define _string_to_int16(s) ((_int16)atoi(s))
#define _string_to_int32(s) ((_int32)atoi(s))
#define _string_to_int64(s) ((_int64)atoi(s))
#define _string_to_uint8(s) ((_uint8)atoi(s))
#define _string_to_uint16(s) ((_uint16)atoi(s))
#define _string_to_uint32(s) ((_uint32)atoi(s))
#define _string_to_uint64(s) ((_uint64)atoi(s))
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

#endif
