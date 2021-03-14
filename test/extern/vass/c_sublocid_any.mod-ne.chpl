// Testing uses of 'c_sublocid_any' as a global non-extern variable.

var c_sublocid_any = "hello";

proc innerNonExtern() {
  writeln("innerNonExtern ", c_sublocid_any);
  cobegin {
    write(""); // dummy
    writeln("innerNonExtern cobegin ", c_sublocid_any);
  }
}
innerNonExtern();

cobegin {
  write(""); // dummy
  writeln("module cobegin NE ", c_sublocid_any);
}
