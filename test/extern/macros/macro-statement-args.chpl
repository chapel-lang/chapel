use CTypes;
require "test-macros-complex.h";

extern proc MACRO_RESET();
extern proc MACRO_ACCUMULATE(p: c_ptr(c_int), x);
extern proc MACRO_DISCARD_SUM(x: c_int);
extern proc readMacroState(): c_int;

var evaluations = 0;
proc nextArg(): c_int {
  evaluations += 1;
  return 3:c_int;
}

MACRO_RESET();
var value: c_int = 5;
MACRO_ACCUMULATE(c_ptrTo(value), nextArg());
assert(value == 8 && readMacroState() == 15 && evaluations == 1);
MACRO_DISCARD_SUM(7:c_int);
writeln("macro statements passed");
