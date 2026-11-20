use CTypes;
require "test-macros-complex.h";

extern proc MACRO_RECORD_RESULT(x: c_int);
extern proc readMacroState(): c_int;

MACRO_RECORD_RESULT(17:c_int);
writeln(readMacroState());
