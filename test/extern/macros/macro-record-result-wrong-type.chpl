use CTypes;
require "test-macros-complex.h";

extern proc MACRO_RECORD_RESULT(x: c_int): c_int;

var result: c_int = MACRO_RECORD_RESULT(17:c_int);
writeln(result);
