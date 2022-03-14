require "header.h";
use CTypes;

extern proc returnIntFromIntArg(): c_int;

proc main() {
  var x = returnIntFromIntArg();
  writeln(x);
}
