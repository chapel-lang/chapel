require "header.h";
use SysCTypes;

extern proc returnIntFromIntArg(arg: real): real;

proc main() {
  var x = returnIntFromIntArg(1.0);
  writeln(x);
}
