require "header.h";
use CTypes;

extern proc returnVoidFromVoidArg(): c_int;

proc main() {
  var x = returnVoidFromVoidArg();
  writeln(x);
}
