use CTypes;
require "test-macros-complex.h";

extern proc MACRO_RECORD_RESULT(x: c_int): macro_result;

MACRO_RECORD_RESULT(17:c_int);
