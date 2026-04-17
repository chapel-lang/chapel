use CTypes;
require "invalid-array-to-scalar.h";
require "invalid-array-to-scalar.c";
extern proc myFunc(arr: c_float);

proc main() {
  var arr = [1.0, 2.0, 3.0]: c_float;
  myFunc(arr);
}
