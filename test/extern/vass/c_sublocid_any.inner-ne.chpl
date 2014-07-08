// Testing uses of 'c_sublocid_any' in nested functions.

proc testNonExtern() {
  var c_sublocid_any = "hello";
  proc innerNonExtern() {
    writeln("innerNonExtern ", c_sublocid_any);
  }
  innerNonExtern();
}

testNonExtern();
