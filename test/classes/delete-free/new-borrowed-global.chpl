module M1 {
  class C { var x: int; }
  var global: borrowed C = (new owned C(1)).borrow();
}

module M2 {
  use M1;
  proc main() {
    writeln(global);
  }
}
