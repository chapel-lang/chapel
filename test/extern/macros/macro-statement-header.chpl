use CTypes;
require "test-macros-complex.h";

extern proc MACRO_RESET();
extern proc readMacroState(): c_int;

MACRO_RESET();
writeln(readMacroState());
