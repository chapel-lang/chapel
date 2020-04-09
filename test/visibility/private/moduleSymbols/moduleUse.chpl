module M1 {
  use M2;

  proc main() {
    var x = 16;
    writeln(secretFunction(x));
  }

}

// This module should be visible to M1.
private module M2 {
  proc secretFunction(a: int) {
    return a*3;
  }

}
