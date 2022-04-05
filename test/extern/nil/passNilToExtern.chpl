use CTypes;

require "passNilToExtern.h";

extern proc foo(ptr: c_ptr(c_size_t));

foo(nil);
