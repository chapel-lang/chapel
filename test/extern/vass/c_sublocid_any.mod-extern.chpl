// Testing uses of 'c_sublocid_any' as a global extern variable.

extern var c_sublocid_any: chpl_sublocID_t;

proc innerExt() {
  writeln("innerExt ", c_sublocid_any);
  cobegin {
    write(""); // dummy
    writeln("innerExt cobegin ", c_sublocid_any);
  }
}
innerExt();

cobegin {
  write(""); // dummy
  writeln("module cobegin Ext ", c_sublocid_any);
}
