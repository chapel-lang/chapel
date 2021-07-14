require "header.h";
use SysCTypes;

extern proc returnIntFromIntArg(): c_int;

proc main() {
  var x = returnIntFromIntArg();
  writeln(x);
}
