// Testing uses of 'c_sublocid_none' as a global non-extern variable.

var c_sublocid_none = "hello";

proc innerNonExtern() {
  writeln("innerNonExtern ", c_sublocid_none);
  cobegin {
    write(""); // dummy
    writeln("innerNonExtern cobegin ", c_sublocid_none);
  }
}
innerNonExtern();

cobegin {
  write(""); // dummy
  writeln("module cobegin NE ", c_sublocid_none);
}
