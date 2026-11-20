use CTypes;
require "test-macros-complex.h";

extern proc MACRO_LOCAL_POINTER(): c_ptr(c_int);
writeln(MACRO_LOCAL_POINTER()[0]);
