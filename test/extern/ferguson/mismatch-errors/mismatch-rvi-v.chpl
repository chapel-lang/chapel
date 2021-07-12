require "header.h";
use SysCTypes;

extern proc returnVoidFromVoidArg(): c_int;

proc main() {
  var x = returnVoidFromVoidArg();
  writeln(x);
}
