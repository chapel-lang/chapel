// Testing uses of 'c_sublocid_any' in nested functions.

proc testExtern() {
  extern const c_sublocid_any: chpl_sublocID_t;
  proc innerExtern() {
    writeln("innerExtern ", c_sublocid_any);
  }
  innerExtern();
}

testExtern();
