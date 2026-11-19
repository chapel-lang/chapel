use CTypes;
require "test-macros-complex.h";

extern record macro_result {}
extern proc MACRO_RECORD_RESULT(x: c_int): macro_result;
extern proc readMacroResultX(result: macro_result): c_int;

const result = MACRO_RECORD_RESULT(17:c_int);
writeln(readMacroResultX(result));
