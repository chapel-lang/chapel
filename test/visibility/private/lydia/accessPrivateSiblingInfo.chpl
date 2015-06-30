module M1 {

  proc main() {
    var x = 16;
    writeln(M2.secretFunction(x));
  }

}

/* This module should not be visible to M1.  Private modules at the top level
   scope should be meaningless. */
private module M2 {
  proc secretFunction(a: int) {
    return a*3;
  }

}
