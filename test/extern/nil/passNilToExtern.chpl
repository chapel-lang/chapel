use CPtr, CTypes;

require "passNilToExtern.h";

extern proc foo(ptr: c_ptr(size_t));

foo(nil);
