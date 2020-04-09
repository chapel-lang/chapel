module M1 {
  proc notASecret(a: int) {
    return a*2;
  }
}

/* This module should not be visible to M1.  Private modules at the top level
   scope should be meaningless. */
private module M2 {
  proc secretFunction(a: int) {
    return a*3;
  }

}
