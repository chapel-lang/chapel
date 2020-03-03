
module M1 {
  record R {
    var x: int;
    proc deinit() {
      writeln("Destroying ", x);
    }
  }

  {
    defer { writeln("defer pre 1"); }
    var one = new R(1);
    ref rone = one;
    defer { writeln("defer post 1"); }
    var two = new R(2);
    ref rtwo = two;
    defer { writeln("defer post 2"); }
  }
}

module M2 {
  use M1;

  proc main() {
    writeln("in main");
  }
}
