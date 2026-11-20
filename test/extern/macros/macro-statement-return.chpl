require "test-macros-complex.h";

extern proc MACRO_CALLER_RETURN();
MACRO_CALLER_RETURN();
writeln("unexpected fallthrough");
