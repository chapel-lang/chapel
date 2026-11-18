use CTypes;
require "test-macros-complex.h";

extern var MACRO_LIMIT: c_int;
extern proc readMacroLimit(): c_int;

writeln(MACRO_LIMIT);
MACRO_LIMIT = 7;
writeln(readMacroLimit());
MACRO_LIMIT += 2;
writeln(readMacroLimit());
