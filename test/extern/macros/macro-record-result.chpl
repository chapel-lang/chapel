use CTypes;
require "test-macros-complex.h";

extern record macro_result {
  var x: c_int;
  var y: c_double;
}

extern proc MACRO_RECORD_RESULT(x: c_int): macro_result;

const result = MACRO_RECORD_RESULT(17:c_int);
writeln(result.x, " ", result.y);
