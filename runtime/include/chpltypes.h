#ifndef _chpltypes_H_
#define _chpltypes_H_

enum _boolean {
  false = 0,
  true = 1
};
typedef long long _integer64;
typedef double _float64;
typedef char* _string;

#define _default_format_read_integer64    "%lld" // long long int
#define _default_format_write_integer64   "%lld" // long long int

#define _default_format_read_float32      "%g"   // float
#define _default_format_write_float32     "%g"   // double
#define _default_format_read_float64      "%lg"  // double
#define _default_format_write_float64     "%g"   // double
#define _default_format_read_float128     "%llg" // long double
#define _default_format_write_float128    "%Lg"  // long double 

#define _default_format_read_string       "%s"   
#define _default_format_write_string      "%s"

#define _default_format_read_enum         "%s"
#define _default_format_write_enum        "%s"

#endif

