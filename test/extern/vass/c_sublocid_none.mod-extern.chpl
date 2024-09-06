// Testing uses of 'c_sublocid_none' as a global extern variable.

extern var c_sublocid_none: chpl_sublocID_t;

proc innerExt() {
  writeln("innerExt ", c_sublocid_none);
  cobegin {
    write(""); // dummy
    writeln("innerExt cobegin ", c_sublocid_none);
  }
}
innerExt();

cobegin {
  write(""); // dummy
  writeln("module cobegin Ext ", c_sublocid_none);
}
