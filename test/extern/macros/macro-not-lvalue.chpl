use CTypes;
require "test-macros-complex.h";

extern var MACRO_NOT_LVALUE: c_int;
MACRO_NOT_LVALUE = 7;
