require 'cast-cstring-to-cvoidptr.h', 'cast-cstring-to-cvoidptr.c';

use CTypes;

extern proc strlen(const s: c_string): c_size_t;
extern proc strlen_voidptr(const s: c_ptr(void)): c_size_t;

config const buf = "Hello, World";
assert(strlen(c_ptrToConst_helper(buf):c_string) == strlen_voidptr(c_ptrToConst_helper(buf):c_string:c_ptr(void)));
