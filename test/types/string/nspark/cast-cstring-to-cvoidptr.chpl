require 'cast-cstring-to-cvoidptr.h', 'cast-cstring-to-cvoidptr.c';

use CTypes;

extern proc strlen(const s: c_string): c_size_t;
extern proc strlen_voidptr(const s: c_void_ptr): c_size_t;

config const buf = "Hello, World";
assert(strlen(buf.c_str()) == strlen_voidptr(buf.c_str():c_void_ptr));
