module M1 {
  module M2 {
    var x: int = 3;
    proc f() {
      writeln(x);
    }
  }
  use M2;
  f();
}

