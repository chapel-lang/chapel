module M1 {
  module M2 {
    var x: int = 3;
    def f() {
      writeln(x);
    }
  }
  use M2;
  f();
}

