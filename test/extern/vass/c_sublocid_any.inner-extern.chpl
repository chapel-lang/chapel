// Testing uses of 'c_sublocid_none' in nested functions.

proc testExtern() {
  extern const c_sublocid_none: chpl_sublocID_t;
  proc innerExtern() {
    writeln("innerExtern ", c_sublocid_none);
  }
  innerExtern();
}

testExtern();
