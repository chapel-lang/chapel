// Testing uses of 'c_sublocid_none' in nested functions.

proc testNonExtern() {
  var c_sublocid_none = "hello";
  proc innerNonExtern() {
    writeln("innerNonExtern ", c_sublocid_none);
  }
  innerNonExtern();
}

testNonExtern();
