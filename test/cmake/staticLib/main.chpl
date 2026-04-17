use CTypes;
require "adder.h";
extern proc add(a: c_int, b: c_int): c_int;
proc main() {
  writeln("Hello, world!");
  writeln("1 + 2 = ", add(1, 2));
}
