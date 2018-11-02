module M1 {
  class C { var x: int; }
  var global: borrowed C = new borrowed C(1);
}

module M2 {
  use M1;
  proc main() {
    writeln(global);
  }
}
