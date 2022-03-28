require "header.h";
use CTypes;

extern proc returnVoidFromVoidArg(arg: c_int): c_int;

proc main() {
  var x = returnVoidFromVoidArg(1);
  writeln(x);
}
