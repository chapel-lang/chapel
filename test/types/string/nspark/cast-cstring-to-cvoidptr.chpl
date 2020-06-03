require 'cast-cstring-to-cvoidptr.h', 'cast-cstring-to-cvoidptr.c';

use SysCTypes;

extern proc strlen(const s: c_string): size_t;
extern proc strlen_voidptr(const s: c_void_ptr): size_t;

config const buf = "Hello, World";
assert(strlen(buf.c_str()) == strlen_voidptr(buf.c_str():c_void_ptr));
