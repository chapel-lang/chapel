require "header.h";
use CTypes;

extern proc returnIntFromIntArg(arg: real): real;

proc main() {
  var x = returnIntFromIntArg(1.0);
  writeln(x);
}
